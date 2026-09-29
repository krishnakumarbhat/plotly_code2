/**
 * @file lcda_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Generic LCDA pre run
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-122580}
 */

#include "lcda_pre_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "lcda_pre_run.c"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}


/**
 * Check that initialization is done properly.
 * \uts{CSCSA-127635} \sdd{SF-6840} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Pre_Run_Init__check_proper_initialization)
{
   /** \arrange Set up LCDA inputs to values other than default. */
   lcda_input.f_lcda_enable     = FBK_TRUE;
   lcda_input.f_bsw_enable      = FBK_TRUE;
   lcda_input.f_cvw_enable      = FBK_TRUE;
   lcda_input.f_slc_enable      = FBK_TRUE;
   lcda_input.f_elc_enable      = FBK_TRUE;
   lcda_input.f_dropback_enable = FBK_TRUE;
   lcda_input.f_fallback_enable = FBK_TRUE;

   /** \action Call function to initialize Pre-run. */
   Lcda_Init_Input(&lcda_input);

   /** \assert Check that flags are set correctly. */
   EXPECT_FALSE(lcda_input.f_lcda_enable);
   EXPECT_FALSE(lcda_input.f_bsw_enable);
   EXPECT_FALSE(lcda_input.f_cvw_enable);
   EXPECT_FALSE(lcda_input.f_slc_enable);
   EXPECT_FALSE(lcda_input.f_elc_enable);
   EXPECT_FALSE(lcda_input.f_dropback_enable);
   EXPECT_FALSE(lcda_input.f_fallback_enable);
}

/**
 * Check that pre run fills core input properly.
 * \uts{CSCSA-127636} \sdd{SF-6836} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Pre_Run__check_correct_fill_of_core_inputs)
{
   /** \arrange Set up LCDA calibration and input. */
   lcda_input.f_lcda_enable     = FBK_TRUE;
   lcda_input.f_bsw_enable      = FBK_TRUE;
   lcda_input.f_cvw_enable      = FBK_TRUE;
   lcda_input.f_slc_enable      = FBK_TRUE;
   lcda_input.f_elc_enable      = FBK_FALSE;
   lcda_input.f_dropback_enable = FBK_TRUE;
   lcda_input.f_fallback_enable = FBK_FALSE;
   cals.k_cvw_ttc               = 2.0f;

   /** \action Call function to test. */
   Lcda_Pre_Run(&lcda_instance, &lcda_input, &fbk_output);

   /** \assert Check that flags are set correctly. */
   EXPECT_TRUE(lcda_input.f_lcda_enable);
   EXPECT_TRUE(lcda_input.f_bsw_enable);
   EXPECT_TRUE(lcda_input.f_cvw_enable);
   EXPECT_TRUE(lcda_input.f_slc_enable);
   EXPECT_FALSE(lcda_input.f_elc_enable);
   EXPECT_TRUE(lcda_input.f_dropback_enable);
   EXPECT_FALSE(lcda_input.f_fallback_enable);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.cvw_ttc_threshold, 2.0f);
}

/**
 * Check proper construction of cvw and bsw initial zones.
 * \uts{CSCSA-127637} \sdd{CSCSA-122793} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Create_Initial_Zones__check_bsw_and_cvw_zones_construction)
{
   /** \arrange Set up LCDA calibration and input. */
   lcda_core_input.p_pa_data = &data;

   /** \action Call function to test. */
   Lcda_Create_Initial_Zones(&lcda_core_input, p_vehicle_data, &cals);

   /** \assert Check that zones are set correctly. */
   EXPECT_EQ(lcda_core_input.bsw_zone_calculation_mode, BSW_ZONE_CALC_FIXED_INPUT);
   for (uint8_t p = FBK_ZERO_UINT; p < LCDA_NUMBER_OF_ZONE_POINTS; ++p)
   {
      EXPECT_TRUE(lcda_core_input.initial_bsw_zone.points[p].x <= FBK_ZERO_F);
      EXPECT_TRUE(lcda_core_input.initial_bsw_zone_hys.points[p].x <= FBK_ZERO_F);
      EXPECT_TRUE(lcda_core_input.initial_cvw_zone.points[p].x <= FBK_ZERO_F);
      EXPECT_TRUE(lcda_core_input.initial_cvw_zone_hys.points[p].x <= FBK_ZERO_F);
      EXPECT_TRUE(lcda_core_input.initial_bsw_zone.points[p].y >= FBK_ZERO_F);
      EXPECT_TRUE(lcda_core_input.initial_bsw_zone_hys.points[p].y >= FBK_ZERO_F);
      EXPECT_TRUE(lcda_core_input.initial_cvw_zone.points[p].y >= FBK_ZERO_F);
      EXPECT_TRUE(lcda_core_input.initial_cvw_zone_hys.points[p].y >= FBK_ZERO_F);
   }
   EXPECT_TRUE(lcda_core_input.initial_bsw_zone.points[FRONT_EGO_SIDE].x > lcda_core_input.initial_bsw_zone.points[REAR_EGO_SIDE].x);
   EXPECT_TRUE(lcda_core_input.initial_bsw_zone.points[FRONT_EGO_SIDE].y
               < lcda_core_input.initial_bsw_zone.points[FRONT_OUTER_SIDE].y);
   EXPECT_TRUE(lcda_core_input.initial_bsw_zone.points[REAR_OUTER_SIDE].x
               < lcda_core_input.initial_bsw_zone.points[FRONT_OUTER_SIDE].x);
   EXPECT_TRUE(lcda_core_input.initial_bsw_zone.points[REAR_OUTER_SIDE].y > lcda_core_input.initial_bsw_zone.points[REAR_EGO_SIDE].y);

   EXPECT_TRUE(lcda_core_input.initial_bsw_zone.points[MIDDLE_EGO_SIDE].x
               <= lcda_core_input.initial_bsw_zone.points[FRONT_EGO_SIDE].x);
   EXPECT_TRUE(lcda_core_input.initial_bsw_zone.points[MIDDLE_OUTER_SIDE].x
               <= lcda_core_input.initial_bsw_zone.points[FRONT_OUTER_SIDE].x);
   EXPECT_TRUE(lcda_core_input.initial_bsw_zone.points[MIDDLE_EGO_SIDE].x >= lcda_core_input.initial_bsw_zone.points[REAR_EGO_SIDE].x);
   EXPECT_TRUE(lcda_core_input.initial_bsw_zone.points[MIDDLE_OUTER_SIDE].x
               >= lcda_core_input.initial_bsw_zone.points[REAR_OUTER_SIDE].x);
}

/**
 * Check that dynamic ttc threshold is disabled.
 * \uts{CSCSA-239265} \sdd{SF-6836} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Pre_Run__check_dynamic_ttc_is_disabled)
{
   /** \arrange Set up LCDA input and calculate factors. */
   lcda_input.hmi_cvw_dyn_ttc = CVW_DYN_TTC_DISABLED;

   /** \action Call function to test. */
   Lcda_Pre_Run(&lcda_instance, &lcda_input, &fbk_output);

   /** \assert Check that flags are set correctly. */
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.cvw_ttc_threshold, cals.k_cvw_ttc);
}


/**
 * Check that dynamic ttc threshold for early alert is set.
 * \uts{CSCSA-239266} \sdd{SF-6836} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Pre_Run__check_dynamic_ttc_early_factors)
{
   /** \arrange Set up LCDA input and calculate factors. */
   lcda_input.hmi_cvw_dyn_ttc = CVW_DYN_TTC_EARLY;
   float32_T speed_factor     = (1.f / (2.0f * cals.k_lcda_cvw_ttc_accel[((uint8_t) lcda_input.hmi_cvw_dyn_ttc) - 1u]));

   /** \action Call function to test. */
   Lcda_Pre_Run(&lcda_instance, &lcda_input, &fbk_output);

   /** \assert Check that flags are set correctly. */
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.cvw_ttc_threshold, cals.k_lcda_cvw_ttc_const[0]);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.cvw_ttc_speed_factor, speed_factor);
}

/**
 * Check that dynamic ttc threshold for normal alert is set.
 * \uts{CSCSA-239267} \sdd{SF-6836} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Pre_Run__check_dynamic_ttc_normal_factors)
{
   /** \arrange Set up LCDA input and calculate factors. */
   lcda_input.hmi_cvw_dyn_ttc = CVW_DYN_TTC_NORMAL;
   float32_T speed_factor     = (1.f / (2.0f * cals.k_lcda_cvw_ttc_accel[((uint8_t) lcda_input.hmi_cvw_dyn_ttc) - 1u]));

   /** \action Call function to test. */
   Lcda_Pre_Run(&lcda_instance, &lcda_input, &fbk_output);

   /** \assert Check that flags are set correctly. */
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.cvw_ttc_threshold, cals.k_lcda_cvw_ttc_const[1]);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.cvw_ttc_speed_factor, speed_factor);
}

/**
 * Check that dynamic ttc threshold for late alert is set.
 * \uts{CSCSA-239268} \sdd{SF-6836} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Pre_Run__check_dynamic_ttc_late_factors)
{
   /** \arrange Set up LCDA input and calculate factors. */
   lcda_input.hmi_cvw_dyn_ttc = CVW_DYN_TTC_LATE;
   float32_T speed_factor     = (1.f / (2.0f * cals.k_lcda_cvw_ttc_accel[((uint8_t) lcda_input.hmi_cvw_dyn_ttc) - 1u]));

   /** \action Call function to test. */
   Lcda_Pre_Run(&lcda_instance, &lcda_input, &fbk_output);

   /** \assert Check that flags are set correctly. */
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.cvw_ttc_threshold, cals.k_lcda_cvw_ttc_const[2]);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.cvw_ttc_speed_factor, speed_factor);
}
