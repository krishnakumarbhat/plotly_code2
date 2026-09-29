/**
 * @file recw_output_debouncer_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is RECW output debouncer test source file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44321}
 */

#include "recw_output_debouncer_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
#include "recw_core_calibration.h"
#include "recw_core_output_t.h"
#include "recw_output_debouncer.c"
#include "recw_persistent_t.h"
#include "recw_types.h"
#include "gtest/gtest_pred_impl.h"
}

/**
 * Test that level 2 alert is not reset by alert qualification function, in the case that no previous alert was present and
 * consecutive alert levels are disabled. Verify that core output and alert level are unchanged. \uts{CSCSA-44322} \sdd{SF-7930}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Output_Debouncer_Test,
       Recw_Apply_Alert_Qualification__sets_RECW_alert_for_internal_alert_level_2_with_no_previous_alert_if_consecutive_level_disabled)
{
   /** \arrange Set up core output (with alert level 2) and persistent data (with no previous alert). Disable logic for consecutive
    * alert levels. */
   recw_core_output.recw_alert_level = RECW_ALERT_ACTIVE_LEVEL_2;
   recw_core_output.recw_id          = 4u;
   recw_core_output.recw_ttc         = 2.0f;

   recw_cals.k_recw_f_only_allow_consecutive_alert_levels = 0u;
   recw_pers.recw_alert_prev_cycle                        = RECW_NO_ALERT;

   /** \action Call function to apply alert qualification. */
   Recw_Apply_Alert_Qualification(&recw_core_output, &recw_pers, &recw_cals);

   /** \assert Verify that core output and alert level are unchanged. */
   EXPECT_EQ(recw_core_output.recw_alert_level, RECW_ALERT_ACTIVE_LEVEL_2);
   EXPECT_EQ(recw_core_output.recw_id, 4u);
   EXPECT_FLOAT_EQ(recw_core_output.recw_ttc, 2.0f);
}

/**
 * Test that level 2 alert is not reset by alert qualification function, in the case that previous alert level 1 and consecutive
 * alert levels are enabled. Verify that core output and alert level are unchanged. \uts{CSCSA-44323} \sdd{SF-7930}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Output_Debouncer_Test,
       Recw_Apply_Alert_Qualification__sets_RECW_alert_for_internal_alert_level_2_with_previous_alert_level_1__if_consecutive_level_enabled)
{
   /** \arrange Set up core output (with alert level 2) and persistent data (with alert level 1). Enable logic for consecutive
    * alert levels. */
   recw_core_output.recw_alert_level = RECW_ALERT_ACTIVE_LEVEL_2;
   recw_core_output.recw_id          = 4u;
   recw_core_output.recw_ttc         = 2.0f;

   recw_cals.k_recw_f_only_allow_consecutive_alert_levels = 1u;
   recw_pers.recw_alert_prev_cycle                        = RECW_ALERT_ACTIVE_LEVEL_1;

   /** \action Call function to apply alert qualification. */
   Recw_Apply_Alert_Qualification(&recw_core_output, &recw_pers, &recw_cals);

   /** \assert Verify that core output and alert level are unchanged. */
   EXPECT_EQ(recw_core_output.recw_alert_level, RECW_ALERT_ACTIVE_LEVEL_2);
   EXPECT_EQ(recw_core_output.recw_id, 4u);
   EXPECT_FLOAT_EQ(recw_core_output.recw_ttc, 2.0f);
}

/**
 * Test that level 2 alert is reset by alert qualification function, in the case that no previous alert was present and consecutive
 * alert levels are enabled. Verify that core output and alert level are reset. \uts{CSCSA-44324} \sdd{SF-7930}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Output_Debouncer_Test,
       Recw_Apply_Alert_Qualification__does_not_set_RECW_alert_for_internal_alert_level_2_with_no_previous_alert_if_consecutive_level_enabled)
{
   /** \arrange Set up core output (with alert level 2) and persistent data (with no previous alert). Enable logic for consecutive
    * alert levels. */
   recw_core_output.recw_alert_level = RECW_ALERT_ACTIVE_LEVEL_2;
   recw_core_output.recw_id          = 4u;
   recw_core_output.recw_ttc         = 2.0f;

   recw_cals.k_recw_f_only_allow_consecutive_alert_levels = 1u;
   recw_pers.recw_alert_prev_cycle                        = RECW_NO_ALERT;

   /** \action Call function to apply alert qualification. */
   Recw_Apply_Alert_Qualification(&recw_core_output, &recw_pers, &recw_cals);

   /** \assert Verify that core output and alert level are reset. */
   EXPECT_EQ(recw_core_output.recw_alert_level, RECW_NO_ALERT);
   EXPECT_EQ(recw_core_output.recw_id, PA_INVALID_OBJ_ID);
   EXPECT_FLOAT_EQ(recw_core_output.recw_ttc, RECW_MAX_TTC);
}

/**
 * Test that level 1 alert is not reset by alert qualification function if qualifying counter is over threshold. Verify that core
 * output and alert level are unchanged in this case. \uts{CSCSA-44325} \sdd{SF-7930} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Output_Debouncer_Test, Recw_Apply_Alert_Qualification__sets_RECW_alert_for_internal_alert_level_1_if_counter_over_threshold)
{
   /** \arrange Set up core output (with alert level 1) and persistent data (with qualifying counter over threshold). */
   recw_pers.recw_alert_qualifying_counter = recw_cals.k_recw_alert_qualifying_cycles;
   recw_core_output.recw_alert_level       = RECW_ALERT_ACTIVE_LEVEL_1;
   recw_core_output.recw_id                = 4u;
   recw_core_output.recw_ttc               = 2.0f;
   recw_pers.recw_alert_prev_cycle         = RECW_NO_ALERT;
   recw_pers.recw_alert_holding_counter    = recw_cals.k_recw_alert_holding_cycles[RECW_INDEX_ALERT_LEVEL_1];

   /** \action Call function to apply alert qualification. */
   Recw_Apply_Alert_Qualification(&recw_core_output, &recw_pers, &recw_cals);

   /** \assert Verify that core output and alert level are unchanged and qualifying counter is incremented. */
   EXPECT_EQ(recw_core_output.recw_alert_level, RECW_ALERT_ACTIVE_LEVEL_1);
   EXPECT_EQ(recw_pers.recw_alert_qualifying_counter, recw_cals.k_recw_alert_qualifying_cycles + 1u);
   EXPECT_EQ(recw_core_output.recw_id, 4u);
   EXPECT_FLOAT_EQ(recw_core_output.recw_ttc, 2.0f);
}

/**
 * Test that level 1 alert is reset by alert qualification function if qualifying counter is below threshold. Verify that core
 * output and alert level are reset in this case. \uts{CSCSA-44326} \sdd{SF-7930} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Output_Debouncer_Test,
       Recw_Apply_Alert_Qualification__does_not_set_RECW_alert_for_internal_alert_level_1_if_counter_below_threshold)
{
   /** \arrange Set up core output (with alert level 1) and persistent data (with qualifying counter below threshold). */
   recw_cals.k_recw_alert_qualifying_cycles = 4u;
   recw_pers.recw_alert_qualifying_counter  = recw_cals.k_recw_alert_qualifying_cycles - 1;
   recw_core_output.recw_alert_level        = RECW_ALERT_ACTIVE_LEVEL_1;
   recw_core_output.recw_id                 = 4u;
   recw_core_output.recw_ttc                = 2.0f;
   recw_pers.recw_alert_prev_cycle          = RECW_NO_ALERT;
   recw_pers.recw_alert_holding_counter     = recw_cals.k_recw_alert_holding_cycles[RECW_INDEX_ALERT_LEVEL_1];

   /** \action Call function to apply alert qualification. */
   Recw_Apply_Alert_Qualification(&recw_core_output, &recw_pers, &recw_cals);

   /** \assert Verify that core output and alert level are reset and qualifying counter is incremented. */
   EXPECT_EQ(recw_core_output.recw_alert_level, RECW_NO_ALERT);
   EXPECT_EQ(recw_pers.recw_alert_qualifying_counter, recw_cals.k_recw_alert_qualifying_cycles);
   EXPECT_EQ(recw_core_output.recw_id, PA_INVALID_OBJ_ID);
   EXPECT_FLOAT_EQ(recw_core_output.recw_ttc, RECW_MAX_TTC);
}

/**
 * Test validating apply alert qualificaton when id is same as id in previous cycle.
 * \uts{CSCSA-204754} \sdd{SF-7930} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Output_Debouncer_Test, Recw_Apply_Alert_Qualification__id_prev_cycle_equal_to_id_in_current_cycle)
{
   /** \arrange Set up core output (with alert level 1) and persistent data (with qualifying counter below threshold). */
   recw_pers.recw_id_prev_cycle = 3u;

   recw_core_output.recw_id                     = 3u;
   recw_core_output.recw_alert_level            = RECW_ALERT_ACTIVE_LEVEL_1;
   recw_core_output.recw_unique_id              = 11u;
   recw_core_output.recw_index                  = 221u;
   recw_core_output.recw_ttc                    = 1.0f;
   recw_core_output.recw_crash_prob_braking     = 0.5f;
   recw_core_output.recw_crash_prob_combined    = 0.5f;
   recw_core_output.recw_crash_prob_steering    = 0.5f;
   recw_core_output.ttc_threshold_alert_level_1 = 0.6f;
   recw_core_output.ttc_threshold_alert_level_2 = 0.5f;

   /** \action Call function to apply alert qualification. */
   Recw_Apply_Alert_Qualification(&recw_core_output, &recw_pers, &recw_cals);

   /** \assert Verify that ouptut has NOT been reseted */
   EXPECT_EQ(recw_core_output.recw_alert_level, RECW_ALERT_ACTIVE_LEVEL_1);
   EXPECT_EQ(recw_core_output.recw_id, 3u);
   EXPECT_EQ(recw_core_output.recw_unique_id, 11u);
   EXPECT_EQ(recw_core_output.recw_index, 221u);
   EXPECT_EQ(recw_core_output.recw_ttc, 1.0f);
   EXPECT_EQ(recw_core_output.recw_crash_prob_braking, 0.5f);
   EXPECT_EQ(recw_core_output.recw_crash_prob_combined, 0.5f);
   EXPECT_EQ(recw_core_output.recw_crash_prob_steering, 0.5f);
   EXPECT_EQ(recw_core_output.ttc_threshold_alert_level_1, 0.6f);
   EXPECT_EQ(recw_core_output.ttc_threshold_alert_level_2, 0.5f);
}

/**
 * Test that a previous alert is held by alert holding function if holding counter is below threshold. Verify that core output and
 * alert level are filled with holded alert. \uts{CSCSA-44327} \sdd{SF-7929} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Output_Debouncer_Test, Recw_Apply_Alert_Holding__holds_RECW_alert_for_no_internal_alert_if_counter_below_threshold)
{
   /** \arrange Set up core output (with no alert) and persistent data (with alert level 1 and holding counter below threshold). */
   recw_pers.recw_alert_holding_counter    = recw_cals.k_recw_alert_holding_cycles[RECW_INDEX_ALERT_LEVEL_1] - 1u;
   recw_pers.recw_alert_qualifying_counter = recw_cals.k_recw_alert_qualifying_cycles;
   recw_core_output.recw_alert_level       = RECW_NO_ALERT;
   recw_pers.recw_index_prev_cycle         = 4u;
   recw_pers.recw_id_prev_cycle            = 7u;
   recw_pers.recw_alert_prev_cycle         = RECW_ALERT_ACTIVE_LEVEL_1;
   recw_pers.recw_ttc_value_hold           = 1.2f;

   /** \action Call function to apply alert holding. */
   Recw_Apply_Alert_Holding(&recw_core_output, &recw_pers, &recw_cals);

   /** \assert Verify that core output and alert level are filled with holded alert and holding counter is incremented. */
   EXPECT_EQ(recw_core_output.recw_alert_level, RECW_ALERT_ACTIVE_LEVEL_1);
   EXPECT_EQ(recw_core_output.recw_index, 4u);
   EXPECT_EQ(recw_core_output.recw_id, 7u);
   EXPECT_EQ(recw_pers.recw_alert_holding_counter, recw_cals.k_recw_alert_holding_cycles[RECW_INDEX_ALERT_LEVEL_1]);
   EXPECT_EQ(recw_pers.recw_ttc_value_hold, 1.2f);
}

/**
 * Test that a previous alert is held by alert holding function if holding counter is below threshold. Verify that core output and
 * alert level are filled with holded alert for alert level 2. \uts{CSCSA-44328} \sdd{SF-7929} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Output_Debouncer_Test, Recw_Apply_Alert_Holding__holds_RECW_alert_level_2_for_no_internal_alert_if_counter_below_threshold)
{
   /** \arrange Set up core output (with no alert) and persistent data (with alert level 2 and holding counter below threshold). */
   recw_pers.recw_alert_holding_counter    = recw_cals.k_recw_alert_holding_cycles[RECW_INDEX_ALERT_LEVEL_2] - 1u;
   recw_pers.recw_alert_qualifying_counter = recw_cals.k_recw_alert_qualifying_cycles;
   recw_core_output.recw_alert_level       = RECW_NO_ALERT;
   recw_pers.recw_index_prev_cycle         = 4u;
   recw_pers.recw_id_prev_cycle            = 7u;
   recw_pers.recw_alert_prev_cycle         = RECW_ALERT_ACTIVE_LEVEL_2;
   recw_pers.recw_ttc_value_hold           = 0.7f;

   /** \action Call function to apply alert holding. */
   Recw_Apply_Alert_Holding(&recw_core_output, &recw_pers, &recw_cals);

   /** \assert Verify that core output and alert level are filled with holded alert and holding counter is incremented. */
   EXPECT_EQ(recw_core_output.recw_alert_level, RECW_ALERT_ACTIVE_LEVEL_2);
   EXPECT_EQ(recw_core_output.recw_index, 4u);
   EXPECT_EQ(recw_core_output.recw_id, 7u);
   EXPECT_EQ(recw_pers.recw_alert_holding_counter, recw_cals.k_recw_alert_holding_cycles[RECW_INDEX_ALERT_LEVEL_2]);
   EXPECT_EQ(recw_pers.recw_ttc_value_hold, 0.7f);
}

/**
 * Test that a previous alert is not held by alert holding function if holding counter is above threshold. Verify that core output
 * and alert level are filled with standard values. \uts{CSCSA-44329} \sdd{SF-7929} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Output_Debouncer_Test, Recw_Apply_Alert_Holding__does_not_hold_RECW_alert_for_no_internal_alert_if_counter_above_threshold)
{
   /** \arrange Set up core output (with no alert) and persistent data (with alert level 1 and holding counter at threshold). */
   recw_pers.recw_alert_holding_counter    = recw_cals.k_recw_alert_holding_cycles[RECW_INDEX_ALERT_LEVEL_1];
   recw_pers.recw_alert_qualifying_counter = recw_cals.k_recw_alert_qualifying_cycles;
   recw_core_output.recw_alert_level       = RECW_NO_ALERT;
   recw_core_output.recw_ttc               = 1.9f;
   recw_pers.recw_index_prev_cycle         = 4u;
   recw_pers.recw_id_prev_cycle            = 7u;
   recw_pers.recw_alert_prev_cycle         = RECW_ALERT_ACTIVE_LEVEL_1;
   recw_pers.recw_ttc_value_hold           = 1.0f;

   /** \action Call function to apply alert holding. */
   Recw_Apply_Alert_Holding(&recw_core_output, &recw_pers, &recw_cals);

   /** \assert Verify that core output and alert level are filled with standard-values and holding counter is incremented. */
   EXPECT_EQ(recw_core_output.recw_alert_level, RECW_NO_ALERT);
   EXPECT_EQ(recw_core_output.recw_index, FBK_ZERO_INT);
   EXPECT_EQ(recw_core_output.recw_id, PA_INVALID_OBJ_ID);
   EXPECT_EQ(recw_pers.recw_alert_holding_counter, 0);
   EXPECT_EQ(recw_pers.recw_ttc_value_hold, recw_core_output.recw_ttc);
}

/**
 * Test Apply_Alert_Holding function when no alert was present in persistent_data
 * \uts{CSCSA-204755} \sdd{SF-7929} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Output_Debouncer_Test, Recw_Apply_Alert_Holding__no_alert_in_persistent_data)
{
   /** \arrange Setup persistent data and holding counter */
   recw_pers.recw_alert_prev_cycle      = RECW_NO_ALERT;
   recw_core_output.recw_alert_level    = RECW_ALERT_ACTIVE_LEVEL_1;
   recw_pers.recw_alert_holding_counter = 2u;

   /** \action Call Alert_Holding function */
   Recw_Apply_Alert_Holding(&recw_core_output, &recw_pers, &recw_cals);

   /** \assert Confirm that alert holding counter was reset */
   EXPECT_EQ(recw_pers.recw_alert_holding_counter, FBK_ZERO_UINT);
}

/**
 * Test that level 1 alert is not reset by alert duration function if warning duration counter is below threshold. Verify that core
 * output and alert level are unchanged in this case. \uts{CSCSA-44330} \sdd{SF-7928} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Output_Debouncer_Test,
       Recw_Apply_Alert_Duration_Check__sets_RECW_alert_for_internal_alert_level_1_if_warning_duration_within_threshold)
{
   /** \arrange Set up core output (with alert level 1) and persistent data (with alert duration counter below threshold). */
   recw_core_output.recw_alert_level     = RECW_ALERT_ACTIVE_LEVEL_1;
   recw_core_output.recw_id              = 4u;
   recw_core_output.recw_ttc             = 2.0f;
   recw_pers.recw_alert_prev_cycle       = RECW_ALERT_ACTIVE_LEVEL_1;
   recw_pers.recw_alert_duration_counter = recw_cals.k_recw_max_cycles_alert_duration[RECW_INDEX_ALERT_LEVEL_1] - 1;

   /** \action Call function to check alert duration. */
   Recw_Apply_Alert_Duration_Check(&recw_core_output, &recw_pers, &recw_cals);

   /** \assert Verify that core output and alert level are unchanged and alert duration counter is incremented. */
   EXPECT_EQ(recw_core_output.recw_alert_level, RECW_ALERT_ACTIVE_LEVEL_1);
   EXPECT_EQ(recw_core_output.recw_id, 4u);
   EXPECT_EQ(recw_pers.recw_alert_duration_counter, recw_cals.k_recw_max_cycles_alert_duration[RECW_INDEX_ALERT_LEVEL_1]);
   EXPECT_FLOAT_EQ(recw_core_output.recw_ttc, 2.0f);
}

/**
 * Test that level 2 alert is not reset by alert duration function if warning duration counter is below threshold. Verify that core
 * output and alert level are unchanged in this case. \uts{CSCSA-44331} \sdd{SF-7928} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Output_Debouncer_Test,
       Recw_Apply_Alert_Duration_Check__sets_RECW_alert_for_internal_alert_level_2_if_warning_duration_within_threshold)
{
   /** \arrange Set up core output (with alert level 2) and persistent data (with alert duration counter below threshold). */
   recw_core_output.recw_alert_level     = RECW_ALERT_ACTIVE_LEVEL_2;
   recw_core_output.recw_id              = 4u;
   recw_core_output.recw_ttc             = 2.0f;
   recw_pers.recw_alert_prev_cycle       = RECW_ALERT_ACTIVE_LEVEL_2;
   recw_pers.recw_alert_duration_counter = recw_cals.k_recw_max_cycles_alert_duration[RECW_INDEX_ALERT_LEVEL_2] - 1;

   /** \action Call function to check alert duration. */
   Recw_Apply_Alert_Duration_Check(&recw_core_output, &recw_pers, &recw_cals);

   /** \assert Verify that core output and alert level are unchanged and alert duration counter is incremented. */
   EXPECT_EQ(recw_core_output.recw_alert_level, RECW_ALERT_ACTIVE_LEVEL_2);
   EXPECT_EQ(recw_core_output.recw_id, 4u);
   EXPECT_EQ(recw_pers.recw_alert_duration_counter, recw_cals.k_recw_max_cycles_alert_duration[RECW_INDEX_ALERT_LEVEL_2]);
   EXPECT_FLOAT_EQ(recw_core_output.recw_ttc, 2.0f);
}

/**
 * Test that level 1 alert is reset by alert duration function if warning duration counter is above threshold. Verify that core
 * output and alert level are reset in this case. \uts{CSCSA-44332} \sdd{SF-7928} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Output_Debouncer_Test,
       Recw_Apply_Alert_Duration_Check__does_not_set_RECW_alert_for_internal_alert_level_1_if_warning_duration_above_threshold)
{
   /** \arrange Set up core output (with alert level 1) and persistent data (with alert duration counter at threshold). */
   recw_pers.recw_alert_qualifying_counter = recw_cals.k_recw_alert_qualifying_cycles;
   recw_core_output.recw_alert_level       = RECW_ALERT_ACTIVE_LEVEL_1;
   recw_core_output.recw_id                = 4u;
   recw_core_output.recw_ttc               = 2.0f;
   recw_pers.recw_alert_prev_cycle         = RECW_ALERT_ACTIVE_LEVEL_1;

   recw_cals.k_recw_max_cycles_alert_duration[RECW_INDEX_ALERT_LEVEL_1] = 60u;
   recw_pers.recw_alert_duration_counter = recw_cals.k_recw_max_cycles_alert_duration[RECW_INDEX_ALERT_LEVEL_1];

   /** \action Call function to check alert duration. */
   Recw_Apply_Alert_Duration_Check(&recw_core_output, &recw_pers, &recw_cals);

   /** \assert Verify that core output, alert level and alert duration counter are reset. */
   EXPECT_EQ(recw_core_output.recw_alert_level, RECW_NO_ALERT);
   EXPECT_EQ(recw_pers.recw_alert_qualifying_counter, FBK_ZERO_INT);
   EXPECT_EQ(recw_core_output.recw_id, PA_INVALID_OBJ_ID);
   EXPECT_EQ(recw_pers.recw_alert_duration_counter, FBK_ZERO_INT);
   EXPECT_FLOAT_EQ(recw_core_output.recw_ttc, RECW_MAX_TTC);
}

/**
 * Test that all counters are reset by debounce alert function if no current or previous alert is present. Verify that all counter
 * are reset. \uts{CSCSA-44333} \sdd{SF-7933} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Output_Debouncer_Test, Recw_Debounce_Alert_Level__resets_counter_if_nothing_is_active)
{
   /** \arrange Set up core output (with no alert) and persistent data (with no alert and all counters greater zero). */
   recw_pers.recw_alert_holding_counter    = 5u;
   recw_pers.recw_alert_duration_counter   = 6u;
   recw_pers.recw_alert_qualifying_counter = 8u;

   recw_core_output.recw_alert_level = RECW_NO_ALERT;
   recw_pers.recw_alert_prev_cycle   = RECW_NO_ALERT;

   /** \action Call function to debounce alert level. */
   Recw_Debounce_Alert_Level(&recw_core_output, &recw_pers, &recw_cals);

   /** \assert Verify that all counter are reset. */
   EXPECT_EQ(recw_pers.recw_alert_holding_counter, FBK_ZERO_INT);
   EXPECT_EQ(recw_pers.recw_alert_duration_counter, FBK_ZERO_INT);
   EXPECT_EQ(recw_pers.recw_alert_qualifying_counter, FBK_ZERO_INT);
}
