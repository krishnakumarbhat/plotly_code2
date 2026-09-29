/**
 * @file recw_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Generic RECW post run tests
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-122477}
 */

#include "recw_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "recw_post_run.c"
}


/**
 * Test that RECW output is filled from core output.
 * \uts{CSCSA-125715} \sdd{SF-7969} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Post_Run_Test, Recw_Post_Run__is_filled_properly)
{
   /** \arrange Set core output with level 1 alert and tracker data. */

   /** \action Call Recw_Update_Output to update output of RECW. */
   Recw_Post_Run(&recw_instance, &recw_input, &recw_output);

   /** \assert Verify that RECW output is filled correctly with tracker data and alert. */
   EXPECT_FLOAT_EQ(recw_output.recw_crash_probability, recw_core_output.recw_crash_prob_combined);
   EXPECT_FLOAT_EQ(recw_output.recw_ttc_s, recw_core_output.recw_ttc);
   EXPECT_EQ(recw_output.recw_id, recw_core_output.recw_id);
   EXPECT_EQ(recw_output.recw_alert_level, recw_core_output.recw_alert_level);
   EXPECT_FLOAT_EQ(recw_output.ttc_threshold_alert_level_1_s, recw_core_output.ttc_threshold_alert_level_1);
   EXPECT_FLOAT_EQ(recw_output.ttc_threshold_alert_level_2_s, recw_core_output.ttc_threshold_alert_level_2);
}
