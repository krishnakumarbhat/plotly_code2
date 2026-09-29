/**
 * @file cta_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Generic CTA post run
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-188369}
 */

#include "cta_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "cta_post_run.c"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}


/**
 * Check that outputs are passed in post run correctly from core output.
 * \uts{CSCSA-188370} \sdd{SF-3947} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Post_Run_Test, Cta_Post_Run__check_output_creation)
{
   /** \arrange Set up cta core output to non default values. */
   cta_core_output.cta_status = CTA_STATUS_ACTIVE;

   cta_core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT]  = 3u;
   cta_core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_RIGHT] = PA_INVALID_OBJ_INDEX;
   data.object_data[3u].id                                  = 5u;
   cta_core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT]     = data.object_data[3u].id;

   cta_core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]         = 7u;
   cta_core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT] = CTA_CRIT_LEVEL_2;
   cta_core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT]     = 0.5f;
   cta_core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT]    = 0.5f;
   /** \action Call function to test. */
   Cta_Post_Run(&cta_instance, &cta_input, &cta_output);

   /** \assert Check that CTA outputs are set correctly. */
   EXPECT_EQ(cta_output.most_critical_object_by_sides[CTA_MODE_REAR][FBK_SIDE_LEFT].id, 5u);
   EXPECT_EQ(cta_output.most_critical_object_by_sides[CTA_MODE_REAR][FBK_SIDE_LEFT].alert_level, CTA_CRIT_LEVEL_2);
   EXPECT_EQ(cta_output.most_critical_object_by_sides[CTA_MODE_REAR][FBK_SIDE_LEFT].ttc_s, 0.5f);
   EXPECT_EQ(cta_output.most_critical_object_by_sides[CTA_MODE_REAR][FBK_SIDE_RIGHT].id, 0u);
   EXPECT_EQ(cta_output.f_cta_enabled, FBK_TRUE);
}

/**
 * Check enabled state when CTA STATUS is not active
 * \uts{CSCSA-293120} \sdd{SF-3947} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Post_Run_Test, Cta_Post_Run__check_enabled_state)
{
   /** \arrange Set up cta core output to non default values. */
   cta_core_output.cta_status = CTA_STATUS_DISABLED;

   /** \action Call function to test. */
   Cta_Post_Run(&cta_instance, &cta_input, &cta_output);

   /** \assert Check that CTA outputs are set correctly. */
   EXPECT_EQ(cta_output.f_cta_enabled, FBK_FALSE);
}
