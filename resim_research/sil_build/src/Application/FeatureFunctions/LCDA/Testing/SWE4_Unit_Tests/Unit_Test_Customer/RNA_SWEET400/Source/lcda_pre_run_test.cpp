/**
 * @file lcda_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for lcda_pre_run.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-43120}
 */

#include "lcda_pre_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

#include "fbk_output.h"

extern "C"
{
#include "fbk_macros.h"
#include "lcda_pre_run.h"
#include "lcda_types.h"
}

/*
 * Check if core inputs are correctly initialised.
 * \uts{CSCSA-43121} \sdd{SF-6836} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Pre_Run)
{
   /** \arrange No setup needed. */
   Fbk_Output_T fbk_output;
   fbk_output.p_pa_data = &data;
   /** \action Call Lcda_Pre_Run to set core inputs */
   Lcda_Pre_Run(&lcda_instance, &lcda_input, &fbk_output);

   /** \assert Check whether core inputs are set correctly. */
   EXPECT_EQ(lcda_core_input.enabled_flags.f_lcda_enabled, FBK_TRUE);
   EXPECT_EQ(lcda_core_input.enabled_flags.f_bsw_enabled, FBK_TRUE);
   EXPECT_EQ(lcda_core_input.enabled_flags.f_cvw_enabled, FBK_TRUE);
   EXPECT_EQ(lcda_core_input.enabled_flags.f_dropback_enabled, FBK_FALSE);
   EXPECT_EQ(lcda_core_input.enabled_flags.f_fallback_enabled, FBK_TRUE);
   EXPECT_EQ(lcda_core_input.bsw_zone_calculation_mode, BSW_ZONE_CALC_FIXED_INPUT);
   EXPECT_EQ(lcda_core_input.lane_width, p_vehicle_data->lane_width);
   EXPECT_EQ(lcda_core_input.lane_center_offset, p_vehicle_data->lane_center_offset);
   EXPECT_EQ(lcda_core_input.warn_settings.cvw_ttc_threshold, cals.k_cvw_ttc);
   EXPECT_EQ(lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone, FBK_FALSE);
}
