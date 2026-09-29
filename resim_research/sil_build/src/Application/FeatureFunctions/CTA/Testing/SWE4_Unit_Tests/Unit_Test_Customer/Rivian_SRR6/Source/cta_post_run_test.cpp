/**
 * @file cta_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for rivian srr6 cta post run
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42110}
 */

#include "cta_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "cta_post_run.c"
}

/**
 * Check mapping of Cta_Post_Run_Init.
 * \uts{CSCSA-42111} \sdd{SF-3954} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Post_Run_Init__resets_output_properly)
{
   /** \arrange Cta_Post_Run_Init does not have inputs, so no operation is required. */
   /** \action Call Cta_Reset_Output, such that cta_output is filled accordingly. */
   Cta_Reset_Output(&cta_output);
   /** \assert Check that cta_output is filled correctly. */
   EXPECT_FLOAT_EQ(cta_output.front_cta_obj_ttc_left, CTA_HIGH_DEFAULT_VAL);
   EXPECT_FLOAT_EQ(cta_output.front_cta_obj_ttc_right, CTA_HIGH_DEFAULT_VAL);
   EXPECT_EQ(cta_output.front_cta_alert_level_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.front_cta_alert_level_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.front_cta_id_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.front_cta_id_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_front_cta_brake_qualifier_left, FBK_FALSE);
   EXPECT_EQ(cta_output.f_front_cta_brake_qualifier_right, FBK_FALSE);
   EXPECT_FLOAT_EQ(cta_output.rear_cta_obj_ttc_left, CTA_HIGH_DEFAULT_VAL);
   EXPECT_FLOAT_EQ(cta_output.rear_cta_obj_ttc_right, CTA_HIGH_DEFAULT_VAL);
   EXPECT_EQ(cta_output.rear_cta_alert_level_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.rear_cta_alert_level_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.rear_cta_id_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.rear_cta_id_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_rear_cta_brake_qualifier_left, FBK_FALSE);
   EXPECT_EQ(cta_output.f_rear_cta_brake_qualifier_right, FBK_FALSE);
   EXPECT_EQ(cta_output.cta_status, RIVIAN_CTA_DISABLED);
   EXPECT_FLOAT_EQ(cta_output.rear_cta_long_intersection_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.rear_cta_long_intersection_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.rear_cta_heading_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.rear_cta_heading_right, FBK_ZERO_F);
   EXPECT_EQ(cta_output.rear_cta_warn_hold_cnt_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.rear_cta_warn_hold_cnt_right, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(cta_output.front_cta_long_intersection_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.front_cta_long_intersection_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.front_cta_heading_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.front_cta_heading_right, FBK_ZERO_F);
   EXPECT_EQ(cta_output.front_cta_warn_hold_cnt_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.front_cta_warn_hold_cnt_right, FBK_ZERO_UINT);
}


/**
 * Check mapping of Cta_Post_Run.
 * \uts{CSCSA-42112} \sdd{SF-3947} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Post_Run__is_filled_properly_from_core_output)
{
   /** \arrange Set up values, which are used in Cta_Post_Run with arbitrary values. */
   Cta_Post_Run_Init(&cta_instance);
   cta_input.f_front_cta_enable = FBK_TRUE;
   cta_input.f_rear_cta_enable  = FBK_TRUE;

   cta_instance.core_output.cta_obj_ttc[CTA_MODE_FRONT][FBK_SIDE_LEFT]            = 3.1f;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_FRONT][FBK_SIDE_RIGHT]           = 2.0f;
   cta_instance.core_output.cta_alert_level[CTA_MODE_FRONT][FBK_SIDE_LEFT]        = CTA_CRIT_LEVEL_1;
   cta_instance.core_output.cta_alert_level[CTA_MODE_FRONT][FBK_SIDE_RIGHT]       = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.cta_id[CTA_MODE_FRONT][FBK_SIDE_LEFT]                 = 4;
   cta_instance.core_output.cta_id[CTA_MODE_FRONT][FBK_SIDE_RIGHT]                = 6;
   cta_instance.core_output.f_brake_qualifier[CTA_MODE_FRONT][FBK_SIDE_LEFT]      = FBK_FALSE;
   cta_instance.core_output.f_brake_qualifier[CTA_MODE_FRONT][FBK_SIDE_RIGHT]     = FBK_TRUE;
   cta_instance.core_output.cta_long_intersection[CTA_MODE_FRONT][FBK_SIDE_LEFT]  = 1.8f;
   cta_instance.core_output.cta_long_intersection[CTA_MODE_FRONT][FBK_SIDE_RIGHT] = 2.1f;
   cta_instance.core_output.cta_heading[CTA_MODE_FRONT][FBK_SIDE_LEFT]            = 1.3f;
   cta_instance.core_output.cta_heading[CTA_MODE_FRONT][FBK_SIDE_RIGHT]           = 0.85f;
   cta_instance.core_output.cta_warn_hold_cnt[CTA_MODE_FRONT][FBK_SIDE_LEFT]      = 3u;
   cta_instance.core_output.cta_warn_hold_cnt[CTA_MODE_FRONT][FBK_SIDE_RIGHT]     = 4u;

   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT]            = 2.5f;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT]           = 3.0f;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT]        = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT]       = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT]                 = 3;
   cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]                = 5;
   cta_instance.core_output.f_brake_qualifier[CTA_MODE_REAR][FBK_SIDE_LEFT]      = FBK_TRUE;
   cta_instance.core_output.f_brake_qualifier[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = FBK_FALSE;
   cta_instance.core_output.cta_long_intersection[CTA_MODE_REAR][FBK_SIDE_LEFT]  = -1.0f;
   cta_instance.core_output.cta_long_intersection[CTA_MODE_REAR][FBK_SIDE_RIGHT] = -2.0f;
   cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_LEFT]            = 0.3f;
   cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_RIGHT]           = 1.5f;
   cta_instance.core_output.cta_warn_hold_cnt[CTA_MODE_REAR][FBK_SIDE_LEFT]      = 1u;
   cta_instance.core_output.cta_warn_hold_cnt[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = 2u;
   cta_instance.core_output.cta_status                                           = CTA_STATUS_ACTIVE;

   /** \action Call Cta_Post_Run such that cta_output is filled accordingly. */
   Cta_Post_Run(&cta_instance, &cta_input, &cta_output);

   /** \assert Check that cta_output is filled correctly. */
   EXPECT_FLOAT_EQ(cta_output.front_cta_obj_ttc_left, 3.1f);
   EXPECT_FLOAT_EQ(cta_output.front_cta_obj_ttc_right, 2.0f);
   EXPECT_EQ(cta_output.front_cta_alert_level_left, CTA_CRIT_LEVEL_1);
   EXPECT_EQ(cta_output.front_cta_alert_level_right, CTA_CRIT_LEVEL_2);
   EXPECT_EQ(cta_output.front_cta_id_left, 4);
   EXPECT_EQ(cta_output.front_cta_id_right, 6);
   EXPECT_EQ(cta_output.f_front_cta_brake_qualifier_left, FBK_FALSE);
   EXPECT_EQ(cta_output.f_front_cta_brake_qualifier_right, FBK_TRUE);
   EXPECT_FLOAT_EQ(cta_output.rear_cta_obj_ttc_left, 2.5f);
   EXPECT_FLOAT_EQ(cta_output.rear_cta_obj_ttc_right, 3.0f);
   EXPECT_EQ(cta_output.rear_cta_alert_level_left, CTA_CRIT_LEVEL_2);
   EXPECT_EQ(cta_output.rear_cta_alert_level_right, CTA_CRIT_LEVEL_2);
   EXPECT_EQ(cta_output.rear_cta_id_left, 3);
   EXPECT_EQ(cta_output.rear_cta_id_right, 5);
   EXPECT_EQ(cta_output.f_rear_cta_brake_qualifier_left, FBK_TRUE);
   EXPECT_EQ(cta_output.f_rear_cta_brake_qualifier_right, FBK_FALSE);
   EXPECT_EQ(cta_output.cta_status, RIVIAN_CTA_ACTIVE);
   EXPECT_FLOAT_EQ(cta_output.rear_cta_long_intersection_left, -1.0f);
   EXPECT_FLOAT_EQ(cta_output.rear_cta_long_intersection_right, -2.0f);
   EXPECT_FLOAT_EQ(cta_output.rear_cta_heading_left, 0.3f);
   EXPECT_FLOAT_EQ(cta_output.rear_cta_heading_right, 1.5f);
   EXPECT_EQ(cta_output.rear_cta_warn_hold_cnt_left, 1u);
   EXPECT_EQ(cta_output.rear_cta_warn_hold_cnt_right, 2u);
   EXPECT_FLOAT_EQ(cta_output.front_cta_long_intersection_left, 1.8f);
   EXPECT_FLOAT_EQ(cta_output.front_cta_long_intersection_right, 2.1f);
   EXPECT_FLOAT_EQ(cta_output.front_cta_heading_left, 1.3f);
   EXPECT_FLOAT_EQ(cta_output.front_cta_heading_right, 0.85f);
   EXPECT_EQ(cta_output.front_cta_warn_hold_cnt_left, 3u);
   EXPECT_EQ(cta_output.front_cta_warn_hold_cnt_right, 4u);
}

/**
 * Check that CTA status is mapped correctly to Rivian specific CTA status for CTA_STATUS_ACTIVE.
 * \uts{CSCSA-42113} \sdd{} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Map_Cta_Status_To_Rivian__works_properly_for_status_active)
{
   /** \arrange Set up CTA status as CTA_STATUS_ACTIVE. */
   Cta_Status_T cta_status = CTA_STATUS_ACTIVE;

   /** \action Call Cta_Map_Cta_Status_To_Rivian to map status. */
   Cta_Rivian_Status_T cta_rivian_status = Cta_Map_Cta_Status_To_Rivian(cta_status);

   /** \assert Check that rivian specific status is RIVIAN_CTA_ACTIVE. */
   EXPECT_EQ(cta_rivian_status, RIVIAN_CTA_ACTIVE);
}

/**
 * Check that CTA status is mapped correctly to Rivian specific CTA status for CTA_STATUS_DISABLED_BY_INPUT.
 * \uts{CSCSA-42114} \sdd{} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Map_Cta_Status_To_Rivian__works_properly_for_status_disabled)
{
   /** \arrange Set up CTA status as CTA_STATUS_DISABLED_BY_INPUT. */
   Cta_Status_T cta_status = CTA_STATUS_DISABLED;

   /** \action Call Cta_Map_Cta_Status_To_Rivian to map status. */
   Cta_Rivian_Status_T cta_rivian_status = Cta_Map_Cta_Status_To_Rivian(cta_status);

   /** \assert Check that rivian specific status is RIVIAN_CTA_DISABLED. */
   EXPECT_EQ(cta_rivian_status, RIVIAN_CTA_DISABLED);
}

/**
 * Check that CTA status is mapped correctly to Rivian specific CTA status for CTA_STATUS_DEACTIVATED_EGO_SPEED.
 * \uts{CSCSA-42115} \sdd{} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Map_Cta_Status_To_Rivian__works_properly_for_status_deactivated_ego_speed)
{
   /** \arrange Set up CTA status as CTA_STATUS_DEACTIVATED_EGO_SPEED. */
   Cta_Status_T cta_status = CTA_STATUS_DEACTIVATED_EGO_SPEED;

   /** \action Call Cta_Map_Cta_Status_To_Rivian to map status. */
   Cta_Rivian_Status_T cta_rivian_status = Cta_Map_Cta_Status_To_Rivian(cta_status);

   /** \assert Check that rivian specific status is RIVIAN_CTA_DEACTIVATED_EGO_SPEED. */
   EXPECT_EQ(cta_rivian_status, RIVIAN_CTA_DEACTIVATED_EGO_SPEED);
}
