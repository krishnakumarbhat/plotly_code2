/**
 * @file lcda_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for lcda_pre_run.c functions
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-43058}
 */

#include "lcda_pre_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "lcda_pre_run.c"
#include "lcda_types.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}


/**
 * Check that mapping in Lcda_Pre_Run is correct.
 * \uts{CSCSA-43059} \sdd{SF-6836} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Pre_Run__check_correct_mapping)
{
   /** \arrange Set up LCDA input and calibration values, which are used in Lcda_Pre_Run with arbitrary values. */
   lcda_input.f_lcda_enable           = FBK_TRUE;
   lcda_input.f_lcda_enable_bsw       = FBK_TRUE;
   lcda_input.f_lcda_enable_cvw       = FBK_TRUE;
   p_vehicle_data->lane_width         = 3.5f;
   p_vehicle_data->lane_center_offset = 0.5f;
   lcda_input.lcda_warntrigger_hmi    = (Lcda_Rivian_Srr6_Hmi_Warntrigger_T) cals.k_lcda_default_warntrigger_hmi;
   cals.k_cvw_ttc                     = 2.5f;

   /** \action Call Lcda_Pre_Run, such that lcda_core_input is filled accordingly. */
   Lcda_Pre_Run(&lcda_instance, &lcda_input, &fbk_output);

   /** \assert Check that lcda_core_input is filled correctly. */
   EXPECT_EQ(lcda_core_input.enabled_flags.f_lcda_enabled, FBK_TRUE);
   EXPECT_EQ(lcda_core_input.enabled_flags.f_bsw_enabled, FBK_TRUE);
   EXPECT_EQ(lcda_core_input.enabled_flags.f_cvw_enabled, FBK_TRUE);
   EXPECT_EQ(lcda_core_input.enabled_flags.f_slc_enabled, FBK_FALSE);
   EXPECT_EQ(lcda_core_input.enabled_flags.f_elc_enabled, FBK_FALSE);
   EXPECT_EQ(lcda_core_input.enabled_flags.f_dropback_enabled, FBK_FALSE);
   EXPECT_EQ(lcda_core_input.enabled_flags.f_fallback_enabled, FBK_TRUE);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.cvw_ttc_threshold, 2.5f);
   EXPECT_EQ(lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone, FBK_FALSE);
   EXPECT_EQ(lcda_core_input.bsw_zone_calculation_mode, BSW_ZONE_CALC_FIXED_INPUT);
   EXPECT_FLOAT_EQ(lcda_core_input.lane_width, 3.5f);
   EXPECT_FLOAT_EQ(lcda_core_input.lane_center_offset, 0.5f);
}

/**
 * Check the CVW data with respect to trailer settings. Existing trailer shall disable CVW functionality.
 * \uts{CSCSA-43060} \sdd{SF-6836} \testtype{negative}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Pre_Run__check_correct_mapping_trailer_attached)
{
   /** \arrange Set up LCDA input and calibration values, which are used in Lcda_Pre_Run with arbitrary values. */
   lcda_input.f_lcda_enable           = FBK_TRUE;
   lcda_input.f_lcda_enable_bsw       = FBK_TRUE;
   lcda_input.f_lcda_enable_cvw       = FBK_TRUE;
   lcda_input.f_trailer_present       = FBK_TRUE;
   lcda_input.trailer_length          = 1.0f;
   lcda_input.trailer_width           = 0.5f;
   lcda_input.trailer_angle           = 0.1f;
   p_vehicle_data->lane_width         = 3.5f;
   p_vehicle_data->lane_center_offset = 0.5f;
   lcda_input.lcda_warntrigger_hmi    = (Lcda_Rivian_Srr6_Hmi_Warntrigger_T) cals.k_lcda_default_warntrigger_hmi;
   cals.k_cvw_ttc                     = 2.5f;

   /** \action Call Lcda_Pre_Run, such that lcda_core_input is filled accordingly. */
   Lcda_Pre_Run(&lcda_instance, &lcda_input, &fbk_output);

   /** \assert Check that lcda_core_input is filled correctly. */
   EXPECT_EQ(lcda_core_input.enabled_flags.f_lcda_enabled, FBK_TRUE);
   EXPECT_EQ(lcda_core_input.enabled_flags.f_bsw_enabled, FBK_TRUE);
   EXPECT_EQ(lcda_core_input.enabled_flags.f_cvw_enabled, FBK_FALSE);
   EXPECT_EQ(lcda_core_input.enabled_flags.f_slc_enabled, FBK_FALSE);
   EXPECT_EQ(lcda_core_input.enabled_flags.f_elc_enabled, FBK_FALSE);
   EXPECT_EQ(lcda_core_input.enabled_flags.f_dropback_enabled, FBK_FALSE);
   EXPECT_EQ(lcda_core_input.enabled_flags.f_fallback_enabled, FBK_TRUE);
   EXPECT_EQ(lcda_core_input.trailer.f_trailer_present, FBK_TRUE);
   EXPECT_FLOAT_EQ(lcda_core_input.trailer.length, 1.0f);
   EXPECT_FLOAT_EQ(lcda_core_input.trailer.width, 0.5f);
   EXPECT_FLOAT_EQ(lcda_core_input.trailer.angle, 0.1f);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.cvw_ttc_threshold, 2.5f);
   EXPECT_EQ(lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone, FBK_FALSE);
   EXPECT_EQ(lcda_core_input.bsw_zone_calculation_mode, BSW_ZONE_CALC_FIXED_INPUT);
   EXPECT_FLOAT_EQ(lcda_core_input.lane_width, 3.5f);
   EXPECT_FLOAT_EQ(lcda_core_input.lane_center_offset, 0.5f);
}

/**
 * Check that warn settings are set correctly for hmi mode late. Use feature input for mapping.
 * \uts{CSCSA-43061} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Hmi_Warn_Settings__sets_correct_warn_settings_for_hmi_mode_late)
{
   /** \arrange Set up LCDA input and calibration values such that warn trigger mode late is used. */
   cals.k_lcda_use_default_warntrigger_hmi = 0u;
   lcda_input.lcda_warntrigger_hmi         = LCDA_RIVIAN_SRR6_WARNTRIGGER_LATE;

   /** \action Call Lcda_Set_Hmi_Warn_Settings to fill LCDA core input from calibration values and LCDA input. */
   Lcda_Set_Hmi_Warn_Settings(&lcda_core_input, &lcda_input, &cals);

   /** \assert Check that warn settings are set according to hmi mode late. */
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.bsw_len_factor, LCDA_RIVIAN_DEFAULT_BSW_ADJUSTMENT_FACTOR);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.cvw_ttc_threshold, cals.k_cvw_ttc + cals.k_cvw_warntrigger_late);
}

/**
 * Check that warn settings are set correctly for hmi mode normal. For this the default setting via calibration shall be used.
 * \uts{CSCSA-43062} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Hmi_Warn_Settings__sets_correct_warn_settings_for_hmi_mode_normal_use_default_by_cal)
{
   /** \arrange Set up LCDA input and calibration values such that hmi mode normal is used. */
   cals.k_lcda_use_default_warntrigger_hmi = 1u;
   cals.k_lcda_default_warntrigger_hmi     = (uint8_t) LCDA_RIVIAN_SRR6_WARNTRIGGER_NORMAL;

   /** \action Call Lcda_Set_Hmi_Warn_Settings to fill LCDA core input from calibration values and LCDA input. */
   Lcda_Set_Hmi_Warn_Settings(&lcda_core_input, &lcda_input, &cals);

   /** \assert Check that warn settings are set according to hmi mode normal. */
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.bsw_len_factor, LCDA_RIVIAN_DEFAULT_BSW_ADJUSTMENT_FACTOR);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.cvw_ttc_threshold, cals.k_cvw_ttc);
}

/**
 * Check that warn settings are set correctly for hmi mode early.
 * \uts{CSCSA-43063} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Hmi_Warn_Settings__sets_correct_warn_settings_for_hmi_mode_early)
{
   /** \arrange Set up LCDA input and calibration values such that hmi mode 2 is used. */
   cals.k_lcda_use_default_warntrigger_hmi = 1u;
   cals.k_lcda_default_warntrigger_hmi     = (uint8_t) LCDA_RIVIAN_SRR6_WARNTRIGGER_EARLY;


   /** \action Call Lcda_Set_Hmi_Warn_Settings to fill LCDA core input from calibration values and LCDA input. */
   Lcda_Set_Hmi_Warn_Settings(&lcda_core_input, &lcda_input, &cals);

   /** \assert Check that warn settings are set according to hmi mode early. */
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.bsw_len_factor, LCDA_RIVIAN_DEFAULT_BSW_ADJUSTMENT_FACTOR);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.cvw_ttc_threshold, cals.k_cvw_ttc + cals.k_cvw_warntrigger_early);
}

/**
 * Check that warn settings are set to hmi mode normal for unknown hmi mode.
 * \uts{CSCSA-43064} \sdd{} \testtype{negative}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Set_Hmi_Warn_Settings__sets_warn_settings_to_hmi_mode_normal_for_unknown_mode)
{
   /** \arrange Set up LCDA input and calibration values such that hmi mode 5 which is not defined is used. */
   cals.k_lcda_use_default_warntrigger_hmi = 1u;
   cals.k_lcda_default_warntrigger_hmi     = 50u;

   /** \action Call Lcda_Pre_Run to fill LCDA core input from calibration values and LCDA input. */
   Lcda_Set_Hmi_Warn_Settings(&lcda_core_input, &lcda_input, &cals);

   /** \assert Check that warn settings are set according to hmi mode 1. */
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.bsw_len_factor, LCDA_RIVIAN_DEFAULT_BSW_ADJUSTMENT_FACTOR);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.cvw_ttc_threshold, cals.k_cvw_ttc);
}
