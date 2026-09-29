/**
 * @file cta_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Generic cta pre run
 *
 * @copyright Copyright (C) 2025 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-278971}
 */

#include "cta_pre_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "cta_pre_run.c"
#include "cta_types.h"
#include "fbk_macros.h"
#include "pa_context.h"
}

/**
 * Check that mapping in cta pre run is done correctly.
 * \uts{CSCSA-278972} \sdd{SF-3945} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Pre_Run_Test, Cta_Pre_Run__check_mapping_between_input_and_core_input)
{
   /** \arrange set cta input to something */
   cta_input.f_cta_enable = FBK_ONE_UINT;

   /** \action execute pre run */
   Cta_Pre_Run(&cta_instance, &cta_input, &fbk_output, &pt_output);

   /** \assert expect mapping is done correctly */
   EXPECT_EQ(cta_instance.core_input.f_cta_switch, FBK_ONE_UINT);
   EXPECT_TRUE(cta_instance.core_input.p_pa_data != nullptr);

   for (uint8_t mode_idx = FBK_ZERO_UINT; mode_idx < CTA_NUM_MODES; mode_idx++)
   {
      for (uint8_t level_idx = FBK_ZERO_UINT; level_idx < CTA_NUM_CRIT_LEVEL; level_idx++)
      {
         EXPECT_FLOAT_EQ(cta_instance.core_input.ttc_criticality_level[mode_idx][level_idx],
                         cta_instance.calibration.k_cta_ttc_criticality_level[mode_idx][level_idx]);
      }
   }
}

/**
 * Check that mapping in cta pre run is done correctly.
 * \uts{CSCSA-278973} \sdd{SF-3945} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Pre_Run_Test, Cta_Pre_Run__check_mapping_cta_stop_mode_ttc)
{
   /** \arrange set cta input to something */
   cta_input.f_cta_enable                         = FBK_ONE_UINT;
   cta_instance.calibration.k_cta_f_stop_mode_ttp = FBK_FALSE;

   /** \action execute pre run */
   Cta_Pre_Run(&cta_instance, &cta_input, &fbk_output, &pt_output);

   /** \assert expect mapping is done correctly */
   EXPECT_EQ(cta_instance.core_input.cta_stop_mode, CTA_STOP_MODE_TTC);
}

/**
 * Check that mapping in cta pre run is done correctly.
 * \uts{CSCSA-278976} \sdd{SF-3945} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Pre_Run_Test, Cta_Pre_Run__check_mapping_cta_stop_mode_ttp)
{
   /** \arrange set cta input to something */
   cta_input.f_cta_enable                         = FBK_ONE_UINT;
   cta_instance.calibration.k_cta_f_stop_mode_ttp = FBK_ONE_UINT;

   /** \action execute pre run */
   Cta_Pre_Run(&cta_instance, &cta_input, &fbk_output, &pt_output);

   /** \assert expect mapping is done correctly */
   EXPECT_EQ(cta_instance.core_input.cta_stop_mode, CTA_STOP_MODE_TTP);
}

/**
 * Check that core inputs are not filled when cta swtich turn off.
 * \uts{CSCSA-293121} \sdd{SF-3945} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Pre_Run_Test, Cta_Pre_Run__cta_switch_off)
{
   /** \arrange set cta enable as zero */
   Cta_Init_Input(&cta_input);
   cta_input.f_cta_enable = FBK_ZERO_UINT;


   /** \action execute pre run */
   Cta_Pre_Run(&cta_instance, &cta_input, &fbk_output, &pt_output);

   /** \assert expect deafult values */
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[0].x, 0.0);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[1].x, 0.0);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[2].x, 0.0);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[3].x, 0.0);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[0].y, 0.0);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[1].y, 0.0);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[2].y, 0.0);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[3].y, 0.0);
   EXPECT_FLOAT_EQ(cta_instance.core_input.ttc_criticality_level[0][0], 0.0);
   EXPECT_FLOAT_EQ(cta_instance.core_input.ttc_criticality_level[0][1], 0.0);
   EXPECT_FLOAT_EQ(cta_instance.core_input.ttc_criticality_level[1][0], 0.0);
   EXPECT_FLOAT_EQ(cta_instance.core_input.ttc_criticality_level[1][1], 0.0);
}
