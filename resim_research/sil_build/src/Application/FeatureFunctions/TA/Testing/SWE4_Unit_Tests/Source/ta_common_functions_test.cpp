/**
 * @file ta_common_functions_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for TA common functions unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-45230}
 */

#include "ta_common_functions_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "ta_common_functions.c"
#include "ta_types.h"
}


/**
 * Set up a scenario with an active alert. Reset TA output for each side individually and verify that alerts are no longer set.
 * \uts{CSCSA-45231} \sdd{SF-8657} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Common_Functions_Test, Ta_Reset_Core_Output__test_resetting_core_output)
{
   /** \arrange Set active alert levels in TA output. */
   ta_core_output.ta_alert_level[FBK_SIDE_LEFT] = TA_ALERT_STATE_LEVEL_1;
   ta_core_output.ta_ttp[FBK_SIDE_LEFT]         = 4.2f;

   ta_core_output.ta_alert_level[FBK_SIDE_RIGHT] = TA_ALERT_STATE_LEVEL_2;
   ta_core_output.ta_ttc[FBK_SIDE_RIGHT]         = 1.2f;

   /** \action Reset TA output for each side individually. */
   Ta_Reset_Core_Output(&ta_core_output, FBK_SIDE_LEFT);
   Ta_Reset_Core_Output(&ta_core_output, FBK_SIDE_RIGHT);

   /** \assert Check that no alert is raised in TA output. */
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_LEFT], TA_ALERT_STATE_NONE);
   EXPECT_EQ(ta_core_output.ta_alert_level[FBK_SIDE_RIGHT], TA_ALERT_STATE_NONE);
}
