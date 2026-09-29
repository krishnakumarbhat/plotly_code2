/**
 * @file ced_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for BMW SRR5 CED pre run
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-126149}
 */

#include "ced_pre_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "ced_pre_run.c"
#include "fbk_macros.h"
#include "pa_reuse.h"
}


/**
 * Check that ced input is correctly initialized.
 * \uts{CSCSA-126150} \sdd{SF-3398} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Pre_Run_Test, Ced_Pre_Run_Init__initialize_ced_input)
{
   /** \arrange set ced input to default */
   ced_input.f_ced_enable     = FBK_FALSE;
   ced_input.f_ced_front_mode = FBK_FALSE;
   ced_input.f_ced_rear_mode  = FBK_FALSE;

   /** \action execute pre run initialization */
   Ced_Init_Input(&ced_input);

   /** \assert expect ced input to be initialized correctly. */
   EXPECT_TRUE(ced_input.f_ced_enable);
   EXPECT_TRUE(ced_input.f_ced_rear_mode);
   EXPECT_TRUE(ced_input.f_ced_front_mode);
   EXPECT_TRUE(object_data != nullptr);
   EXPECT_TRUE(p_vehicle_data != nullptr);
}


/**
 * Check that mapping in ced pre run is done correctly.
 * \uts{CSCSA-126151} \sdd{SF-3402} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Pre_Run_Test, Ced_Pre_Run__check_mapping_between_input_and_core_input)
{
   /** \arrange set ced input to something */
   ced_input.f_ced_enable     = FBK_TRUE;
   ced_input.f_ced_front_mode = FBK_TRUE;
   ced_input.f_ced_rear_mode  = FBK_FALSE;

   /* set Core CED inputs to FALSE or NULL */
   ced_core_input.f_ced_enable     = FBK_FALSE;
   ced_core_input.f_ced_front_mode = FBK_FALSE;
   ced_core_input.f_ced_rear_mode  = FBK_FALSE;
   ced_core_input.p_pa_data        = nullptr;
   ced_core_input.p_pt_output      = nullptr;

   /** \action execute pre run */
   Ced_Pre_Run(&ced_instance, &ced_input, &pt_output, &fbk_output);

   /** \assert expect mapping is done correctly */
   EXPECT_TRUE(ced_core_input.f_ced_enable);
   EXPECT_TRUE(ced_core_input.f_ced_front_mode);
   EXPECT_FALSE(ced_core_input.f_ced_rear_mode);
   EXPECT_TRUE(ced_core_input.p_pa_data != nullptr);
}

/**
 * Check that mapping in ced pre run is done correctly.
 * \uts{CSCSA-206694} \sdd{SF-3402} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Pre_Run_Test, Ced_Pre_Run__check_mapping_between_input_and_core_input_speed_below_threshold)
{
   /** \arrange set ced input to something */
   ced_input.f_ced_enable     = FBK_TRUE;
   ced_input.f_ced_front_mode = FBK_TRUE;
   ced_input.f_ced_rear_mode  = FBK_FALSE;

   /* set Core CED inputs to FALSE or NULL */
   ced_core_input.f_ced_enable     = FBK_FALSE;
   ced_core_input.f_ced_front_mode = FBK_FALSE;
   ced_core_input.f_ced_rear_mode  = FBK_FALSE;
   ced_core_input.p_pa_data        = nullptr;
   ced_core_input.p_pt_output      = nullptr;
   p_vehicle_data->host_speed      = FBK_ZERO_F;

   /** \action execute pre run */
   Ced_Pre_Run(&ced_instance, &ced_input, &pt_output, &fbk_output);

   /** \assert expect mapping is done correctly */
   EXPECT_TRUE(ced_core_input.f_ced_enable);
   EXPECT_TRUE(ced_core_input.f_ced_front_mode);
   EXPECT_FALSE(ced_core_input.f_ced_rear_mode);
   EXPECT_TRUE(ced_core_input.p_pa_data != nullptr);
}

/**
 * Check that mapping in ced pre run is done correctly.
 * \uts{CSCSA-206695} \sdd{SF-3402} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Pre_Run_Test, Ced_Pre_Run__check_mapping_between_input_and_core_input_speed_above_threshold)
{
   /** \arrange set ced input to something */
   ced_input.f_ced_enable     = FBK_TRUE;
   ced_input.f_ced_front_mode = FBK_TRUE;
   ced_input.f_ced_rear_mode  = FBK_FALSE;

   /* set Core CED inputs to FALSE or NULL */
   ced_core_input.f_ced_enable     = FBK_FALSE;
   ced_core_input.f_ced_front_mode = FBK_FALSE;
   ced_core_input.f_ced_rear_mode  = FBK_FALSE;
   ced_core_input.p_pa_data        = nullptr;
   ced_core_input.p_pt_output      = nullptr;
   p_vehicle_data->host_speed      = 10.0f;

   /** \action execute pre run */
   Ced_Pre_Run(&ced_instance, &ced_input, &pt_output, &fbk_output);

   /** \assert expect mapping is done correctly */
   EXPECT_FALSE(ced_core_input.f_ced_enable);
   EXPECT_TRUE(ced_core_input.f_ced_front_mode);
   EXPECT_FALSE(ced_core_input.f_ced_rear_mode);
   EXPECT_TRUE(ced_core_input.p_pa_data != nullptr);
}

/**
 * Check that mapping in ced pre run is done correctly.
 * \uts{CSCSA-206741} \sdd{SF-3402} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Pre_Run_Test, Ced_Pre_Run__check_mapping_between_input_and_core_input_ced_disabled)
{
   /** \arrange set ced input to something */
   ced_input.f_ced_enable     = FBK_FALSE;
   ced_input.f_ced_front_mode = FBK_TRUE;
   ced_input.f_ced_rear_mode  = FBK_FALSE;

   /* set Core CED inputs to FALSE or NULL */
   ced_core_input.f_ced_enable     = FBK_FALSE;
   ced_core_input.f_ced_front_mode = FBK_FALSE;
   ced_core_input.f_ced_rear_mode  = FBK_FALSE;
   ced_core_input.p_pa_data        = nullptr;
   ced_core_input.p_pt_output      = nullptr;
   p_vehicle_data->host_speed      = FBK_ZERO_F;

   /** \action execute pre run */
   Ced_Pre_Run(&ced_instance, &ced_input, &pt_output, &fbk_output);

   /** \assert expect mapping is done correctly */
   EXPECT_FALSE(ced_core_input.f_ced_enable);
   EXPECT_TRUE(ced_core_input.f_ced_front_mode);
   EXPECT_FALSE(ced_core_input.f_ced_rear_mode);
   EXPECT_TRUE(ced_core_input.p_pa_data != nullptr);
}

/**
 * Check that mapping in ced pre run is done correctly.
 * \uts{CSCSA-206750} \sdd{SF-3402} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Pre_Run_Test, Ced_Pre_Run__check_mapping_between_input_and_core_input_ced_disabled_above_threshold)
{
   /** \arrange set ced input to something */
   ced_input.f_ced_enable     = FBK_FALSE;
   ced_input.f_ced_front_mode = FBK_TRUE;
   ced_input.f_ced_rear_mode  = FBK_FALSE;

   /* set Core CED inputs to FALSE or NULL */
   ced_core_input.f_ced_enable     = FBK_FALSE;
   ced_core_input.f_ced_front_mode = FBK_FALSE;
   ced_core_input.f_ced_rear_mode  = FBK_FALSE;
   ced_core_input.p_pa_data        = nullptr;
   ced_core_input.p_pt_output      = nullptr;
   p_vehicle_data->host_speed      = -10.0f;

   /** \action execute pre run */
   Ced_Pre_Run(&ced_instance, &ced_input, &pt_output, &fbk_output);

   /** \assert expect mapping is done correctly */
   EXPECT_FALSE(ced_core_input.f_ced_enable);
   EXPECT_TRUE(ced_core_input.f_ced_front_mode);
   EXPECT_FALSE(ced_core_input.f_ced_rear_mode);
   EXPECT_TRUE(ced_core_input.p_pa_data != nullptr);
}