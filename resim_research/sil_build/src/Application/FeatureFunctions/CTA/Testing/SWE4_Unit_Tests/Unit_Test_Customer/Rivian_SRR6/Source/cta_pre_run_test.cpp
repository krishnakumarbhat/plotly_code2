/**
 * @file cta_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for rivian cta pre run
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42116}
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
 * Check mapping of Cta_Pre_Run.
 * \uts{CSCSA-42117} \sdd{SF-3945} \testtype{positive}
 */
TEST_F(Cta_Pre_Run_Test, Cta_Pre_Run__mapping)
{
   /** \arrange Set up CTA input, such that CTA is enabled. */
   cta_input.f_cta_enable = 1u;

   /** \action Call Cta_Pre_Run_Test, such that cta_core_input is filled accordingly. */
   Cta_Pre_Run(&cta_instance, &cta_input, &fbk_output, &pt_output);

   /** \assert Check that cta_core_input is filled correctly. */
   EXPECT_EQ(cta_instance.core_input.f_cta_switch, FBK_TRUE);
}


/**
 * Check that warn settings are set correctly for hmi mode late. Use feature input for mapping.
 * \uts{CSCSA-42118} \sdd{CSCSA-27731} \testtype{positive}
 */
TEST_F(Cta_Pre_Run_Test, Cta_Set_Hmi_Warn_Settings__sets_correct_warn_settings_for_hmi_mode_late)
{
   uint8_t level_idx;
   uint8_t mode_idx;

   /** \arrange Set up CTA input such that warn trigger mode late is used. */
   cta_input.cta_warntrigger_hmi = CTA_RIVIAN_SRR6_WARNTRIGGER_LATE;

   /** \action Call Cta_Set_Hmi_Warn_Settings to fill CTA core input from calibration values and CTA input. */
   Cta_Set_Hmi_Warn_Settings(&cta_instance.core_input, &cta_input, &cta_instance.calibration);

   /** \assert Check that warn settings are set according to hmi mode late. */
   for (mode_idx = FBK_ZERO_UINT; mode_idx < (uint8_t) CTA_NUM_MODES; mode_idx++)
   {
      for (level_idx = FBK_ZERO_UINT; level_idx < CTA_NUM_CRIT_LEVEL; level_idx++)
      {
         EXPECT_FLOAT_EQ(cta_instance.core_input.ttc_criticality_level[mode_idx][level_idx],
                         cta_instance.calibration.k_cta_ttc_warntrigger_late);
      }
   }
}

/**
 * Check that warn settings are set correctly for hmi mode normal. Use feature input for mapping.
 * \uts{CSCSA-42119} \sdd{CSCSA-27731} \testtype{positive}
 */
TEST_F(Cta_Pre_Run_Test, Cta_Set_Hmi_Warn_Settings__sets_correct_warn_settings_for_hmi_mode_normal)
{
   uint8_t level_idx;
   uint8_t mode_idx;

   /** \arrange Set up CTA input such that warn trigger mode normal is used. */
   cta_input.cta_warntrigger_hmi = CTA_RIVIAN_SRR6_WARNTRIGGER_NORMAL;

   /** \action Call Cta_Set_Hmi_Warn_Settings to fill CTA core input from calibration values and CTA input. */
   Cta_Set_Hmi_Warn_Settings(&cta_instance.core_input, &cta_input, &cta_instance.calibration);

   /** \assert Check that warn settings are set according to hmi mode normal. */
   for (mode_idx = FBK_ZERO_UINT; mode_idx < (uint8_t) CTA_NUM_MODES; mode_idx++)
   {
      for (level_idx = FBK_ZERO_UINT; level_idx < CTA_NUM_CRIT_LEVEL; level_idx++)
      {
         EXPECT_FLOAT_EQ(cta_instance.core_input.ttc_criticality_level[mode_idx][level_idx],
                         cta_instance.calibration.k_cta_ttc_criticality_level[CTA_MODE_REAR][CTA_CRIT_LEVEL_1 - 1u]);
      }
   }
}

/**
 * Check that warn settings are set correctly for hmi mode early. Use feature input for mapping.
 * \uts{CSCSA-42120} \sdd{CSCSA-27731} \testtype{positive}
 */
TEST_F(Cta_Pre_Run_Test, Cta_Set_Hmi_Warn_Settings__sets_correct_warn_settings_for_hmi_mode_early)
{
   uint8_t level_idx;
   uint8_t mode_idx;

   /** \arrange Set up CTA input such that warn trigger mode early is used. */
   cta_input.cta_warntrigger_hmi = CTA_RIVIAN_SRR6_WARNTRIGGER_EARLY;

   /** \action Call Cta_Set_Hmi_Warn_Settings to fill CTA core input from calibration values and CTA input. */
   Cta_Set_Hmi_Warn_Settings(&cta_instance.core_input, &cta_input, &cta_instance.calibration);

   /** \assert Check that warn settings are set according to hmi mode early. */
   for (mode_idx = FBK_ZERO_UINT; mode_idx < (uint8_t) CTA_NUM_MODES; mode_idx++)
   {
      for (level_idx = FBK_ZERO_UINT; level_idx < CTA_NUM_CRIT_LEVEL; level_idx++)
      {
         EXPECT_FLOAT_EQ(cta_instance.core_input.ttc_criticality_level[mode_idx][level_idx],
                         cta_instance.calibration.k_cta_ttc_warntrigger_early);
      }
   }
}

/**
 * Check that warn settings are set correctly for unknown input. Use feature input for mapping.
 * \uts{CSCSA-42121} \sdd{CSCSA-27731} \testtype{negative}
 */
TEST_F(Cta_Pre_Run_Test, Cta_Set_Hmi_Warn_Settings__sets_correct_warn_settings_for_hmi_mode_unknown)
{
   uint8_t level_idx;
   uint8_t mode_idx;

   /** \arrange Set up CTA input such that undefined mode is used. */
   cta_input.cta_warntrigger_hmi = (Cta_Rivian_Srr6_Hmi_Warntrigger_T) 8u;

   /** \action Call Cta_Set_Hmi_Warn_Settings to fill CTA core input from calibration values and CTA input. */
   Cta_Set_Hmi_Warn_Settings(&cta_instance.core_input, &cta_input, &cta_instance.calibration);

   /** \assert Check that warn settings are set according to hmi mode unknown. */
   for (mode_idx = FBK_ZERO_UINT; mode_idx < (uint8_t) CTA_NUM_MODES; mode_idx++)
   {
      for (level_idx = FBK_ZERO_UINT; level_idx < CTA_NUM_CRIT_LEVEL; level_idx++)
      {
         EXPECT_FLOAT_EQ(cta_instance.core_input.ttc_criticality_level[mode_idx][level_idx],
                         cta_instance.calibration.k_cta_ttc_criticality_level[CTA_MODE_REAR][CTA_CRIT_LEVEL_1 - 1u]);
      }
   }
}

/**
 * Check that CTA zone are set correctly.
 * \uts{CSCSA-42122} \sdd{CSCSA-27732} \testtype{positive}
 */
TEST_F(Cta_Pre_Run_Test, Cta_Construct_Zone__rear)
{
   /** \arrange Set up CTA input such that undefined mode is used. */
   cta_input.f_front_cta_enable = FBK_TRUE;
   cta_input.f_rear_cta_enable  = FBK_FALSE;

   /** \action Call Cta_Set_Hmi_Warn_Settings to fill CTA core input from calibration values and CTA input. */
   Cta_Construct_Zone(&(cta_instance.core_input.cta_zone), &cta_input, &cta_instance.calibration, p_vehicle_data);

   /** \assert Check that warn settings are set according to hmi mode unknown. */
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[0].x, 7.0f);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[0].y, 0.915f);

   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[1].x, 7.0f);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[1].y, 25.915f);

   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[2].x, -11.881968f);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[2].y, 25.915f);

   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[3].x, 0.0f);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[3].y, 0.915f);
}

/**
 * Check that CTA zone are set correctly.
 * \uts{CSCSA-42123} \sdd{CSCSA-27732} \testtype{positive}
 */
TEST_F(Cta_Pre_Run_Test, Cta_Construct_Zone__front)
{
   /** \arrange Set up CTA input such that undefined mode is used. */
   cta_input.f_front_cta_enable = FBK_FALSE;
   cta_input.f_rear_cta_enable  = FBK_TRUE;

   /** \action Call Cta_Set_Hmi_Warn_Settings to fill CTA core input from calibration values and CTA input. */
   Cta_Construct_Zone(&(cta_instance.core_input.cta_zone), &cta_input, &cta_instance.calibration, p_vehicle_data);

   /** \assert Check that warn settings are set according to hmi mode unknown. */
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[0].x, -4.65f);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[0].y, 0.915f);

   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[1].x, 7.0416913f);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[1].y, 25.915f);

   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[2].x, -11.65f);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[2].y, 25.915f);

   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[3].x, -11.65f);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[3].y, 0.915f);
}

/**
 * Check that CTA zone are set correctly.
 * \uts{CSCSA-71107} \sdd{CSCSA-27735} \testtype{positive}
 */
TEST_F(Cta_Pre_Run_Test, Cta_Construct_Rear_Front_Zone_general)
{
   /** \arrange Set up CTA input such that undefined mode is used. */
   cta_input.f_front_cta_enable = FBK_TRUE;
   cta_input.f_rear_cta_enable  = FBK_TRUE;

   /** \action Call Cta_Set_Hmi_Warn_Settings to fill CTA core input from calibration values and CTA input. */
   Cta_Construct_Rear_Front_Zone(&(cta_instance.core_input.cta_zone), &cta_instance.calibration, p_vehicle_data);

   /** \assert Check that warn settings are set according to hmi mode unknown. */
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[0].x, 7.0f);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[0].y, 0.915f);

   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[1].x, 7.0f);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[1].y, 25.915f);

   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[2].x, -11.65f);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[2].y, 25.915f);

   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[3].x, -11.65f);
   EXPECT_FLOAT_EQ(cta_instance.core_input.cta_zone.points[3].y, 0.915f);
}
