/**
 * @file lcda_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Honda_SRR6 LCDA post run
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-83463}
 */

#include "lcda_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "lcda_output_t.h"
#include "lcda_post_run.c"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}


/**
 * Check that outputs are reset to their defaults.
 * \uts{CSCSA-83808} \sdd{SF-6993} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Reset_Output__check_default_mapping)
{
   /** \arrange Set up LCDA output to non default. */
   lcda_output.f_lcda_enabled  = 1u;
   lcda_output.f_bsw_enabled   = 1u;
   lcda_output.f_cvw_enabled   = 1u;
   lcda_output.bsw_alert_left  = 1u;
   lcda_output.bsw_alert_right = 1u;
   lcda_output.cvw_alert_left  = 1u;
   lcda_output.cvw_alert_right = 1u;
   lcda_output.cvw_id_left     = 1u;
   lcda_output.cvw_id_right    = 1u;
   lcda_output.bsw_id_left     = 1u;
   lcda_output.bsw_id_right    = 1u;
   lcda_output.cvw_ttc_left    = 2.0f;
   lcda_output.cvw_ttc_right   = 2.0f;

   /** \action Call function to test. */
   Lcda_Reset_Output(&lcda_output);

   /** \assert Check that defaults are set correctly. */
   EXPECT_EQ(lcda_output.f_lcda_enabled, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.f_bsw_enabled, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.f_cvw_enabled, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bsw_alert_left, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.bsw_alert_right, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.cvw_alert_left, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.cvw_alert_right, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.cvw_id_left, PA_INVALID_OBJ_ID);
   EXPECT_EQ(lcda_output.cvw_id_right, PA_INVALID_OBJ_ID);
   EXPECT_EQ(lcda_output.bsw_id_left, PA_INVALID_OBJ_ID);
   EXPECT_EQ(lcda_output.bsw_id_right, PA_INVALID_OBJ_ID);
   EXPECT_FLOAT_EQ(lcda_output.cvw_ttc_left, LCDA_LKA_NA_TTC);
   EXPECT_FLOAT_EQ(lcda_output.cvw_ttc_right, LCDA_LKA_NA_TTC);
}

/**
 * Check that outputs are reset to their defaults by initialization routine.
 * \uts{CSCSA-83911} \sdd{SF-6987} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run_Init__check_no_fatal_error)
{
   /** \arrange Set up LCDA output to non default. */

   /** \action Call function to test. */

   /** \assert Check that defaults are set correctly. */
   ASSERT_NO_FATAL_FAILURE(Lcda_Post_Run_Init(););
}

/**
 * Check that in case of an active alert for any submodule the corresponding object ID is filled to the LCDA output.
 * \uts{CSCSA-109117} \sdd{SF-6838} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run__fills_alert_id_to_lcda_output_for_active_alerts)
{
   /** \arrange Set up LCDA core output such that alerts are present for all submodules on both sides. */
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_2;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_2;

   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_LEFT]  = 1u;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_LEFT]  = 2u;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_RIGHT] = 5u;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_RIGHT] = 6u;

   lcda_core_output.bsw_core_output.bsw_index[FBK_SIDE_LEFT]  = 1u;
   lcda_core_output.cvw_core_output.cvw_index[FBK_SIDE_LEFT]  = 2u;
   lcda_core_output.bsw_core_output.bsw_index[FBK_SIDE_RIGHT] = 5u;
   lcda_core_output.cvw_core_output.cvw_index[FBK_SIDE_RIGHT] = 6u;

   object_data->curvi_vel_rel.x = 5.0f;

   data.object_data[1u].status = PA_OBJ_STATUS_MATURE;
   data.object_data[2u].status = PA_OBJ_STATUS_MATURE;
   data.object_data[5u].status = PA_OBJ_STATUS_MATURE;
   data.object_data[6u].status = PA_OBJ_STATUS_MATURE;

   data.object_data[1u].id = 1u;
   data.object_data[2u].id = 2u;
   data.object_data[5u].id = 5u;
   data.object_data[6u].id = 6u;

   data.object_data[1u].curvi_vel_rel.x = 10.0f;
   data.object_data[2u].curvi_vel_rel.x = 10.0f;
   data.object_data[5u].curvi_vel_rel.x = 10.0f;
   data.object_data[6u].curvi_vel_rel.x = 10.0f;

   lcda_core_output.lcda_status = LCDA_STATUS_ACTIVE;

   /** \action Call Lcda_Post_Run to fill LCDA output from core output. */
   Lcda_Post_Run(&lcda_instance, &lcda_input, &lcda_output, &fbk_output);

   /** \assert Check that object IDs and alerts are set correctly for all active alerts. */
   EXPECT_EQ(lcda_output.bsw_id_left, lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.cvw_id_left, lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_LEFT]);
   EXPECT_EQ(lcda_output.bsw_id_right, lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.cvw_id_right, lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_RIGHT]);
   EXPECT_EQ(lcda_output.bsw_alert_left, 1u);
   EXPECT_EQ(lcda_output.cvw_alert_left, 1u);
   EXPECT_EQ(lcda_output.bsw_alert_right, 2u);
   EXPECT_EQ(lcda_output.cvw_alert_right, 2u);
}


/**
 * Check if the level 2 alert is set correctly. Objects in CVW and BSW have different id, bsw distance > cvw disntance
 * \uts{CSCSA-109118} \sdd{SF-6999} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Should_Alert_Level_Two_Be_Turned_On__cvw_closer)
{
   /** \arrange Set LKA objectss */
   uint8_t bsw_idx = 0u;
   uint8_t cvw_idx = 1u;

   data.object_data[cvw_idx].id              = bsw_idx + 1;
   data.object_data[bsw_idx].id              = cvw_idx + 1;
   data.object_data[cvw_idx].curvi_pos.x     = -13.5f;
   data.object_data[bsw_idx].curvi_pos.x     = -1.5f;
   data.object_data[cvw_idx].curvi_pos.y     = 1.5f;
   data.object_data[bsw_idx].curvi_pos.y     = 6.5f;
   data.object_data[cvw_idx].curvi_vel_rel.x = 10.0f;
   data.object_data[bsw_idx].curvi_vel_rel.x =
      data.object_data[cvw_idx].curvi_vel_rel.x - customer_cals.k_honda_min_relative_speed_for_alert_level_two - EPSILON;
   p_vehicle_data->host_length = 3.0f;

   customer_cals.k_honda_is_slide_through_zone_considered_for_cvw_alert_level_two = FBK_FALSE;

   /** \action Call function to check level 2 alert */
   boolean_T result = Lcda_Should_Alert_Level_Two_Be_Turned_On(&data, &customer_cals, cvw_idx, bsw_idx);

   /** \assert Check result */
   EXPECT_TRUE(result);
}

/**
 * Check if the level 2 alert is set correctly. Objects in CVW and BSW have different id, bsw distance < cvw disntance
 * \uts{CSCSA-109119} \sdd{SF-6999} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Should_Alert_Level_Two_Be_Turned_On__bsw_closer)
{
   /** \arrange Set LKA objectss */
   uint8_t bsw_idx = 0u;
   uint8_t cvw_idx = 1u;

   data.object_data[cvw_idx].id              = bsw_idx + 1;
   data.object_data[bsw_idx].id              = cvw_idx + 1;
   data.object_data[cvw_idx].id              = 1u;
   data.object_data[bsw_idx].id              = 2u;
   data.object_data[cvw_idx].curvi_pos.y     = 6.5f;
   data.object_data[bsw_idx].curvi_pos.y     = 1.5f;
   data.object_data[cvw_idx].curvi_vel_rel.x = 5u;
   data.object_data[bsw_idx].curvi_vel_rel.x = 4u;

   p_vehicle_data->host_length           = 3.0f;
   data.object_data[bsw_idx].curvi_pos.x = data.object_data[bsw_idx].curvi_pos.y - p_vehicle_data->host_length;
   data.object_data[cvw_idx].curvi_pos.x = data.object_data[cvw_idx].curvi_pos.y - p_vehicle_data->host_length;

   customer_cals.k_honda_is_slide_through_zone_considered_for_cvw_alert_level_two = FBK_FALSE;

   /** \action Call function to check level 2 alert */
   boolean_T result = Lcda_Should_Alert_Level_Two_Be_Turned_On(&data, &customer_cals, cvw_idx, bsw_idx);

   /** \assert Check result */
   EXPECT_FALSE(result);
}

/**
 * Check if the level 2 alert is set correctly. Objects in CVW and BSW have different id, bsw speed > cvw speed
 * \uts{CSCSA-109120} \sdd{SF-6999} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Should_Alert_Level_Two_Be_Turned_On__bsw_faster)
{
   /** \arrange Set LKA objectss */
   uint8_t bsw_idx = 0u;
   uint8_t cvw_idx = 1u;


   data.object_data[cvw_idx].id              = bsw_idx + 1;
   data.object_data[bsw_idx].id              = cvw_idx + 1;
   data.object_data[cvw_idx].curvi_pos.y     = 1.5f;
   data.object_data[bsw_idx].curvi_pos.y     = 6.5f;
   data.object_data[cvw_idx].curvi_vel_rel.x = 4u;
   data.object_data[bsw_idx].curvi_vel_rel.x = 5u;

   p_vehicle_data->host_length           = 3.0f;
   data.object_data[bsw_idx].curvi_pos.x = data.object_data[bsw_idx].curvi_pos.y - p_vehicle_data->host_length;
   data.object_data[cvw_idx].curvi_pos.x = data.object_data[cvw_idx].curvi_pos.y - p_vehicle_data->host_length;

   customer_cals.k_honda_is_slide_through_zone_considered_for_cvw_alert_level_two = FBK_FALSE;

   /** \action Call function to check level 2 alert */
   boolean_T result = Lcda_Should_Alert_Level_Two_Be_Turned_On(&data, &customer_cals, cvw_idx, bsw_idx);

   /** \assert Check result */
   EXPECT_FALSE(result);
}

/**
 * Check if the hold time is calculated correctly. General case for right side
 * \uts{CSCSA-109121} \sdd{SF-6999} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Hold_Alert_Time_And_Index__general_case_right)
{
   /** \arrange Set speed and position */
   uint8_t bsw_index = FBK_ZERO_UINT;
   Lcda_Post_Run_Init();
   data.object_data[bsw_index].curvi_vel_rel.x = FBK_ONE_F;
   data.object_data[bsw_index].curvi_pos.x     = -2.0f;
   data.object_data[bsw_index].curvi_pos.y     = 3.f;

   /** \action Call function to set hold time and object id */
   Lcda_Set_Hold_Alert_Time_And_Index(&data, &cals, p_vehicle_data, bsw_index, &lcda_output, HONDA_SIDE_BSW_RIGHT, &lcda_core_output);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.hold_obj_index[HONDA_SIDE_BSW_RIGHT], FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.predicted_exit_time[HONDA_SIDE_BSW_RIGHT], 2.0f);
}

/**
 * Check if the hold time is calculated correctly. General case for left side.
 * \uts{CSCSA-109122} \sdd{SF-6999} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Hold_Alert_Time_And_Index__general_case_left)
{
   /** \arrange Set speed and position */
   uint8_t bsw_index = FBK_ZERO_UINT;
   Lcda_Post_Run_Init();
   data.object_data[bsw_index].curvi_vel_rel.x = FBK_ONE_F;
   data.object_data[bsw_index].curvi_pos.x     = -2.0f;
   data.object_data[bsw_index].curvi_pos.y     = -3.f;
   HONDA_SIDE_T honda_side                     = HONDA_SIDE_BSW_LEFT;
   /** \action Call function to set hold time and object id */
   Lcda_Set_Hold_Alert_Time_And_Index(&data, &cals, p_vehicle_data, bsw_index, &lcda_output, honda_side, &lcda_core_output);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.hold_obj_index[HONDA_SIDE_BSW_LEFT], FBK_ZERO_UINT);
   EXPECT_EQ(lcda_output.predicted_exit_time[HONDA_SIDE_BSW_LEFT], 2.0f);
}

/**
 * Check if the hold time is calculated correctly. Object velocity too low.
 * \uts{CSCSA-109123} \sdd{SF-6999} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Hold_Alert_Time_And_Index__too_low_velocity)
{
   /** \arrange Set speed and position */
   uint8_t bsw_index = FBK_ZERO_UINT;
   /** \action Call function to set hold time and object id */
   HONDA_SIDE_T honda_side = HONDA_SIDE_BSW_RIGHT;
   /** \action Call function to set hold time and object id */
   Lcda_Set_Hold_Alert_Time_And_Index(&data, &cals, p_vehicle_data, bsw_index, &lcda_output, honda_side, &lcda_core_output);


   /** \assert Check result */
   EXPECT_EQ(lcda_output.hold_obj_index[HONDA_SIDE_BSW_RIGHT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(lcda_output.predicted_exit_time[HONDA_SIDE_BSW_RIGHT], -LCDA_LKA_NA_TTC);
}

/**
 * Check if the hold time is calculated correctly. Object crosses front line outside the zone.
 * \uts{CSCSA-109124} \sdd{SF-6999} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Hold_Alert_Time_And_Index__cross_point_out_max)
{
   /** \arrange Set speed and position */
   uint8_t bsw_index = FBK_ZERO_UINT;
   Lcda_Post_Run_Init();
   data.object_data[bsw_index].curvi_vel_rel.x = -11.f;
   data.object_data[bsw_index].curvi_vel_rel.x = -10.f;
   data.object_data[bsw_index].curvi_pos.x     = -10.f;
   data.object_data[bsw_index].curvi_pos.y     = 3.f;
   data.object_data[bsw_index].curvi_vel_rel.y = 10.f;
   HONDA_SIDE_T honda_side                     = HONDA_SIDE_BSW_RIGHT;

   /** \action Call function to set hold time and object id */
   Lcda_Set_Hold_Alert_Time_And_Index(&data, &cals, p_vehicle_data, bsw_index, &lcda_output, honda_side, &lcda_core_output);


   /** \assert Check result */
   EXPECT_EQ(lcda_output.hold_obj_index[HONDA_SIDE_BSW_RIGHT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(lcda_output.predicted_exit_time[HONDA_SIDE_BSW_RIGHT], -LCDA_LKA_NA_TTC);
}

/**
 * Check if the hold time is calculated correctly. Object crosses front line outside the zone.
 * \uts{CSCSA-109125} \sdd{SF-6999} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Hold_Alert_Time_And_Index__cross_point_out_min)
{
   /** \arrange Set speed and position */
   uint8_t bsw_index = FBK_ZERO_UINT;
   Lcda_Post_Run_Init();
   data.object_data[bsw_index].curvi_vel_rel.x = -11.f;
   data.object_data[bsw_index].curvi_vel_rel.x = -10.f;
   data.object_data[bsw_index].curvi_pos.x     = -10.f;
   data.object_data[bsw_index].curvi_pos.y     = 0.2f;
   data.object_data[bsw_index].curvi_vel_rel.y = 0.f;
   HONDA_SIDE_T honda_side                     = HONDA_SIDE_BSW_RIGHT;

   /** \action Call function to set hold time and object id */
   Lcda_Set_Hold_Alert_Time_And_Index(&data, &cals, p_vehicle_data, bsw_index, &lcda_output, honda_side, &lcda_core_output);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.hold_obj_index[HONDA_SIDE_BSW_RIGHT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(lcda_output.predicted_exit_time[HONDA_SIDE_BSW_RIGHT], -LCDA_LKA_NA_TTC);
}

/**
 * Check if hold times are set correctly. Here holding is expected because ego has slowed down
 * \uts{CSCSA-109126} \sdd{SF-7001} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Honda_Alert_Before_Reset_Bsw__hold_slow_down)
{
   /** \arrange Set speed and position */
   Lcda_Post_Run_Init();
   lcda_output.hold_time[HONDA_SIDE_BSW_LEFT]                = -LCDA_LKA_NA_TTC;
   lcda_output.predicted_exit_time[HONDA_SIDE_BSW_LEFT]      = 5.0;
   lcda_output.bsw_alert_left                                = 1u;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT] = LCDA_ALERT_STATE_NONE;
   lcda_output.hold_obj_index[HONDA_SIDE_BSW_LEFT]           = 2u;
   data.object_data[2u].status                               = PA_OBJ_STATUS_MATURE;

   lcda_output.hold_time[HONDA_SIDE_BSW_RIGHT]                = -LCDA_LKA_NA_TTC;
   lcda_output.predicted_exit_time[HONDA_SIDE_BSW_RIGHT]      = 6.0;
   lcda_output.bsw_alert_right                                = 1u;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;
   lcda_output.hold_obj_index[HONDA_SIDE_BSW_RIGHT]           = 3u;
   data.object_data[3u].status                                = PA_OBJ_STATUS_MATURE;

   p_vehicle_data->host_speed = cals.k_lcda_host_activation_speed_min - cals.k_lcda_host_activation_speed_min_hys - EPSILON;

   customer_cals.k_honda_srr6_enable_alert_hold_due_slow_down  = FBK_TRUE;
   customer_cals.k_honda_srr6_enable_alert_hold_due_out_of_fov = FBK_FALSE;

   /** \action Call function to set hold time and object id */
   Lcda_Hold_Honda_Alert_Before_Reset_Bsw(&lcda_output, &lcda_core_output, *p_vehicle_data, &cals, &customer_cals, &data);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_BSW_LEFT], 5.0);
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_BSW_RIGHT], 6.0);
}

/**
 * Check if hold times are set correctly. Here holding occures due to object leaving fov and holding time is limited by calibration
 * \uts{CSCSA-109127} \sdd{SF-7001} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Honda_Alert_Before_Reset_Bsw__fov_hold_limit_by_cal)
{
   /** \arrange Set speed and position */
   Lcda_Post_Run_Init();
   lcda_output.hold_time[HONDA_SIDE_BSW_LEFT]                = -LCDA_LKA_NA_TTC;
   lcda_output.predicted_exit_time[HONDA_SIDE_BSW_LEFT]      = 5.0;
   lcda_output.bsw_alert_left                                = 1u;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT] = LCDA_ALERT_STATE_NONE;
   lcda_output.hold_obj_index[HONDA_SIDE_BSW_LEFT]           = 2u;
   data.object_data[2u].status                               = PA_OBJ_STATUS_INVALID;

   lcda_output.hold_time[HONDA_SIDE_BSW_RIGHT]                = -LCDA_LKA_NA_TTC;
   lcda_output.predicted_exit_time[HONDA_SIDE_BSW_RIGHT]      = 6.0;
   lcda_output.bsw_alert_right                                = 1u;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;
   lcda_output.hold_obj_index[HONDA_SIDE_BSW_RIGHT]           = 3u;
   data.object_data[3u].status                                = PA_OBJ_STATUS_INVALID;

   customer_cals.k_honda_srr6_enable_alert_hold_due_slow_down  = FBK_FALSE;
   customer_cals.k_honda_srr6_enable_alert_hold_due_out_of_fov = FBK_TRUE;
   customer_cals.k_honda_max_hold_time_after_out_of_fov        = 1.0f;
   p_vehicle_data->host_speed                                  = customer_cals.k_honda_ego_speed_stop_holding + EPSILON;

   /** \action Call function to set hold time and object id */
   Lcda_Hold_Honda_Alert_Before_Reset_Bsw(&lcda_output, &lcda_core_output, *p_vehicle_data, &cals, &customer_cals, &data);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_BSW_LEFT], 1.0);
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_BSW_RIGHT], 1.0);
}

/**
 * Check if hold times are set correctly. Here holding is not occuring because the objects do not change status
 * \uts{} \sdd{SF-7001} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Honda_Alert_Before_Reset_Bsw__fov_no_hold_status_mature)
{
   /** \arrange Set speed and position */
   Lcda_Post_Run_Init();
   lcda_output.hold_time[HONDA_SIDE_BSW_LEFT]                = -LCDA_LKA_NA_TTC;
   lcda_output.predicted_exit_time[HONDA_SIDE_BSW_LEFT]      = 5.0;
   lcda_output.bsw_alert_left                                = 1u;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT] = LCDA_ALERT_STATE_NONE;
   lcda_output.hold_obj_index[HONDA_SIDE_BSW_LEFT]           = 2u;
   data.object_data[2u].status                               = PA_OBJ_STATUS_MATURE;

   lcda_output.hold_time[HONDA_SIDE_BSW_RIGHT]                = -LCDA_LKA_NA_TTC;
   lcda_output.predicted_exit_time[HONDA_SIDE_BSW_RIGHT]      = 6.0;
   lcda_output.bsw_alert_right                                = 1u;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;
   lcda_output.hold_obj_index[HONDA_SIDE_BSW_RIGHT]           = 3u;
   data.object_data[3u].status                                = PA_OBJ_STATUS_MATURE;

   customer_cals.k_honda_srr6_enable_alert_hold_due_slow_down  = FBK_FALSE;
   customer_cals.k_honda_srr6_enable_alert_hold_due_out_of_fov = FBK_TRUE;
   customer_cals.k_honda_max_hold_time_after_out_of_fov        = 1.0f;
   p_vehicle_data->host_speed                                  = customer_cals.k_honda_ego_speed_stop_holding + EPSILON;

   /** \action Call function to set hold time and object id */
   Lcda_Hold_Honda_Alert_Before_Reset_Bsw(&lcda_output, &lcda_core_output, *p_vehicle_data, &cals, &customer_cals, &data);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_BSW_LEFT], -LCDA_LKA_NA_TTC);
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_BSW_RIGHT], -LCDA_LKA_NA_TTC);
}

/**
 * Check if hold times are set correctly. Here holding time is lower than threshold for object lost in fov
 * \uts{} \sdd{SF-7001} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Honda_Alert_Before_Reset_Bsw__fov_hold_time_lower_than_limit)
{
   /** \arrange Set speed and position */
   Lcda_Post_Run_Init();
   lcda_output.hold_time[HONDA_SIDE_BSW_LEFT]                = -LCDA_LKA_NA_TTC;
   lcda_output.predicted_exit_time[HONDA_SIDE_BSW_LEFT]      = 5.0f;
   lcda_output.bsw_alert_left                                = 1u;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT] = LCDA_ALERT_STATE_NONE;
   lcda_output.hold_obj_index[HONDA_SIDE_BSW_LEFT]           = 2u;
   data.object_data[2u].status                               = PA_OBJ_STATUS_INVALID;

   lcda_output.hold_time[HONDA_SIDE_BSW_RIGHT]                = -LCDA_LKA_NA_TTC;
   lcda_output.predicted_exit_time[HONDA_SIDE_BSW_RIGHT]      = 6.0f;
   lcda_output.bsw_alert_right                                = 1u;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;
   lcda_output.hold_obj_index[HONDA_SIDE_BSW_RIGHT]           = 3u;
   data.object_data[3u].status                                = PA_OBJ_STATUS_INVALID;

   customer_cals.k_honda_srr6_enable_alert_hold_due_slow_down  = FBK_FALSE;
   customer_cals.k_honda_srr6_enable_alert_hold_due_out_of_fov = FBK_TRUE;
   customer_cals.k_honda_max_hold_time_after_out_of_fov        = 10.0f;
   p_vehicle_data->host_speed                                  = customer_cals.k_honda_ego_speed_stop_holding + EPSILON;

   /** \action Call function to set hold time and object id */
   Lcda_Hold_Honda_Alert_Before_Reset_Bsw(&lcda_output, &lcda_core_output, *p_vehicle_data, &cals, &customer_cals, &data);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_BSW_LEFT], 5.0f);
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_BSW_RIGHT], 6.0f);
}

/**
 * Check if hold times are set correctly. Here holding due to leaving fov is disabled by calibration
 * \uts{} \sdd{SF-7001} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Honda_Alert_Before_Reset_Bsw__fov_hold_disable)
{
   /** \arrange Set speed and position */
   Lcda_Post_Run_Init();
   lcda_output.hold_time[HONDA_SIDE_BSW_LEFT]                = -LCDA_LKA_NA_TTC;
   lcda_output.predicted_exit_time[HONDA_SIDE_BSW_LEFT]      = 5.0;
   lcda_output.bsw_alert_left                                = 1u;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT] = LCDA_ALERT_STATE_NONE;
   lcda_output.hold_obj_index[HONDA_SIDE_BSW_LEFT]           = 2u;
   data.object_data[2u].status                               = PA_OBJ_STATUS_INVALID;

   lcda_output.hold_time[HONDA_SIDE_BSW_RIGHT]                = -LCDA_LKA_NA_TTC;
   lcda_output.predicted_exit_time[HONDA_SIDE_BSW_RIGHT]      = 6.0;
   lcda_output.bsw_alert_right                                = 1u;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;
   lcda_output.hold_obj_index[HONDA_SIDE_BSW_RIGHT]           = 3u;
   data.object_data[3u].status                                = PA_OBJ_STATUS_INVALID;

   customer_cals.k_honda_srr6_enable_alert_hold_due_slow_down  = FBK_FALSE;
   customer_cals.k_honda_srr6_enable_alert_hold_due_out_of_fov = FBK_FALSE;
   p_vehicle_data->host_speed                                  = customer_cals.k_honda_ego_speed_stop_holding + EPSILON;

   /** \action Call function to set hold time and object id */
   Lcda_Hold_Honda_Alert_Before_Reset_Bsw(&lcda_output, &lcda_core_output, *p_vehicle_data, &cals, &customer_cals, &data);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_BSW_LEFT], -LCDA_LKA_NA_TTC);
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_BSW_RIGHT], -LCDA_LKA_NA_TTC);
}

/**
 * Check alert drop down is detected correctly, here drop on left side side is expected;
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Has_Alert_Drop_Down__drop_on_bsw_left)
{
   /** \arrange core and client alerts */
   boolean_T result;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT] = LCDA_ALERT_STATE_NONE;
   lcda_output.bsw_alert_left                                = FBK_ONE_UINT;
   lcda_output.hold_time[HONDA_SIDE_BSW_LEFT]                = -1.5f;

   /** \action Call function to check if alert was dropped */
   result = Lcda_Has_Alert_Drop_Down(&lcda_output, &lcda_core_output, HONDA_SIDE_BSW_LEFT);

   /** \assert Check result */
   EXPECT_TRUE(result);
}

/**
 * Check alert drop down is detected correctly, here no drop on left side side is expected due to positive hold time;
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Has_Alert_Drop_Down__no_drop_on_bsw_left)
{
   /** \arrange core and client alerts */
   boolean_T result;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT] = LCDA_ALERT_STATE_NONE;
   lcda_output.bsw_alert_left                                = FBK_ONE_UINT;
   lcda_output.hold_time[HONDA_SIDE_BSW_LEFT]                = 1.5f;

   /** \action Call function to check if alert was dropped */
   result = Lcda_Has_Alert_Drop_Down(&lcda_output, &lcda_core_output, HONDA_SIDE_BSW_LEFT);

   /** \assert Check result */
   EXPECT_FALSE(result);
}

/**
 * Check alert drop down is detected correctly, here invalid side is passed;
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Has_Alert_Drop_Down__invalid_side)
{
   /** \arrange core and client alerts */
   boolean_T result;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT] = LCDA_ALERT_STATE_NONE;
   lcda_output.bsw_alert_left                                = FBK_ONE_UINT;
   lcda_output.hold_time[HONDA_SIDE_BSW_LEFT]                = -1.5f;

   /** \action Call function to check if alert was dropped */
   result = Lcda_Has_Alert_Drop_Down(&lcda_output, &lcda_core_output, HONDA_SIDE_NUMBER);

   /** \assert Check result */
   EXPECT_FALSE(result);
}

/**
 * Check if hold times are set correctly.
 * \uts{CSCSA-109128} \sdd{SF-7001} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Honda_Alert_Before_Reset_Bsw__hold_mature_object)
{
   /** \arrange Set speed and position */
   Lcda_Post_Run_Init();
   lcda_output.hold_time[HONDA_SIDE_BSW_LEFT]                = -LCDA_LKA_NA_TTC;
   lcda_output.predicted_exit_time[HONDA_SIDE_BSW_LEFT]      = 5.0;
   lcda_output.bsw_alert_left                                = 1u;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT] = LCDA_ALERT_STATE_NONE;
   lcda_output.hold_obj_index[HONDA_SIDE_BSW_LEFT]           = 2u;
   data.object_data[2u].status                               = PA_OBJ_STATUS_MATURE;

   lcda_output.hold_time[HONDA_SIDE_BSW_RIGHT]                = -LCDA_LKA_NA_TTC;
   lcda_output.predicted_exit_time[HONDA_SIDE_BSW_RIGHT]      = 6.0;
   lcda_output.bsw_alert_right                                = 1u;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;
   lcda_output.hold_obj_index[HONDA_SIDE_BSW_RIGHT]           = 3u;
   data.object_data[3u].status                                = PA_OBJ_STATUS_MATURE;

   p_vehicle_data->host_speed = cals.k_lcda_host_activation_speed_min - cals.k_lcda_host_activation_speed_min_hys - EPSILON;

   customer_cals.k_honda_srr6_enable_alert_hold_due_slow_down = FBK_TRUE;

   /** \action Call function to set hold time and object id */
   Lcda_Hold_Honda_Alert_Before_Reset_Bsw(&lcda_output, &lcda_core_output, *p_vehicle_data, &cals, &customer_cals, &data);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_BSW_LEFT], 5.0);
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_BSW_RIGHT], 6.0);
}

/**
 * Check if hold times are set correctly.
 * \uts{CSCSA-109129} \sdd{SF-7001} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Honda_Alert_Before_Reset_Bsw__mature_object_speed_to_high)
{
   /** \arrange Set speed and position */
   Lcda_Post_Run_Init();
   lcda_output.hold_time[HONDA_SIDE_BSW_LEFT]                = -LCDA_LKA_NA_TTC;
   lcda_output.predicted_exit_time[HONDA_SIDE_BSW_LEFT]      = 5.0;
   lcda_output.bsw_alert_left                                = 1u;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT] = LCDA_ALERT_STATE_NONE;
   lcda_output.hold_obj_index[HONDA_SIDE_BSW_LEFT]           = 2u;
   data.object_data[2u].status                               = PA_OBJ_STATUS_MATURE;

   lcda_output.hold_time[HONDA_SIDE_BSW_RIGHT]                = -LCDA_LKA_NA_TTC;
   lcda_output.predicted_exit_time[HONDA_SIDE_BSW_RIGHT]      = 6.0;
   lcda_output.bsw_alert_right                                = 1u;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;
   lcda_output.hold_obj_index[HONDA_SIDE_BSW_RIGHT]           = 3u;
   data.object_data[3u].status                                = PA_OBJ_STATUS_MATURE;

   p_vehicle_data->host_speed = cals.k_lcda_host_activation_speed_min - cals.k_lcda_host_activation_speed_min_hys + EPSILON;

   /** \action Call function to set hold time and object id */
   Lcda_Hold_Honda_Alert_Before_Reset_Bsw(&lcda_output, &lcda_core_output, *p_vehicle_data, &cals, &customer_cals, &data);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_BSW_LEFT], -LCDA_LKA_NA_TTC);
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_BSW_RIGHT], -LCDA_LKA_NA_TTC);
}

/**
 * Check if hold times are set correctly.
 * \uts{CSCSA-109130} \sdd{SF-7001} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Honda_Alert_Before_Reset_Bsw__core_alert_on)
{
   /** \arrange Set speed and position */
   Lcda_Post_Run_Init();
   lcda_output.hold_time[HONDA_SIDE_BSW_LEFT]                = -LCDA_LKA_NA_TTC;
   lcda_output.predicted_exit_time[HONDA_SIDE_BSW_LEFT]      = 5.0;
   lcda_output.bsw_alert_left                                = 1u;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT] = LCDA_ALERT_STATE_LEVEL_1;
   lcda_output.hold_obj_index[HONDA_SIDE_BSW_LEFT]           = 2u;
   data.object_data[2u].status                               = PA_OBJ_STATUS_INVALID;

   lcda_output.hold_time[HONDA_SIDE_BSW_RIGHT]                = -LCDA_LKA_NA_TTC;
   lcda_output.predicted_exit_time[HONDA_SIDE_BSW_RIGHT]      = 6.0;
   lcda_output.bsw_alert_right                                = 1u;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_1;
   lcda_output.hold_obj_index[HONDA_SIDE_BSW_RIGHT]           = 3u;
   data.object_data[3u].status                                = PA_OBJ_STATUS_INVALID;

   p_vehicle_data->host_speed = cals.k_lcda_host_activation_speed_min - cals.k_lcda_host_activation_speed_min_hys - EPSILON;

   /** \action Call function to set hold time and object id */
   Lcda_Hold_Honda_Alert_Before_Reset_Bsw(&lcda_output, &lcda_core_output, *p_vehicle_data, &cals, &customer_cals, &data);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_BSW_LEFT], -LCDA_LKA_NA_TTC);
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_BSW_RIGHT], -LCDA_LKA_NA_TTC);
}
/**
 * Check if hold times are set correctly.
 * \uts{CSCSA-109131} \sdd{SF-7000} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Honda_Alert_Before_Reset_Cvw__hold)
{
   /** \arrange Set speed and position */
   Lcda_Post_Run_Init();
   lcda_output.hold_time[HONDA_SIDE_CVW_LEFT]                = -LCDA_LKA_NA_TTC;
   lcda_output.predicted_exit_time[HONDA_SIDE_CVW_LEFT]      = 5.0;
   lcda_output.cvw_alert_left                                = 1u;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT] = LCDA_ALERT_STATE_NONE;
   lcda_output.hold_obj_index[HONDA_SIDE_CVW_LEFT]           = 2u;
   data.object_data[2u].status                               = PA_OBJ_STATUS_INVALID;

   lcda_output.hold_time[HONDA_SIDE_CVW_RIGHT]                = -LCDA_LKA_NA_TTC;
   lcda_output.predicted_exit_time[HONDA_SIDE_CVW_RIGHT]      = 6.0;
   lcda_output.cvw_alert_right                                = 1u;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;
   lcda_output.hold_obj_index[HONDA_SIDE_CVW_RIGHT]           = 3u;
   data.object_data[3u].status                                = PA_OBJ_STATUS_INVALID;

   p_vehicle_data->host_speed = cals.k_lcda_host_activation_speed_min - cals.k_lcda_host_activation_speed_min_hys - EPSILON;

   customer_cals.k_honda_srr6_enable_alert_hold_due_slow_down = FBK_TRUE;

   /** \action Call function to set hold time and object id */
   Lcda_Hold_Honda_Alert_Before_Reset_Cvw(&lcda_output, &lcda_core_output, *p_vehicle_data, &cals, &customer_cals);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_CVW_LEFT], 5.0);
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_CVW_RIGHT], 6.0);
}

/**
 * Check if hold times are set correctly.
 * \uts{CSCSA-109132} \sdd{SF-7000} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Honda_Alert_Before_Reset_Cvw__mature_object_speed_to_high)
{
   /** \arrange Set speed and position */
   Lcda_Post_Run_Init();
   lcda_output.hold_time[HONDA_SIDE_CVW_LEFT]                = -LCDA_LKA_NA_TTC;
   lcda_output.predicted_exit_time[HONDA_SIDE_CVW_LEFT]      = 5.0;
   lcda_output.cvw_alert_left                                = 1u;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT] = LCDA_ALERT_STATE_NONE;
   lcda_output.hold_obj_index[HONDA_SIDE_CVW_LEFT]           = 2u;
   data.object_data[2u].status                               = PA_OBJ_STATUS_MATURE;

   lcda_output.hold_time[HONDA_SIDE_CVW_RIGHT]                = -LCDA_LKA_NA_TTC;
   lcda_output.predicted_exit_time[HONDA_SIDE_CVW_RIGHT]      = 6.0;
   lcda_output.cvw_alert_right                                = 1u;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;
   lcda_output.hold_obj_index[HONDA_SIDE_CVW_RIGHT]           = 3u;
   data.object_data[3u].status                                = PA_OBJ_STATUS_MATURE;

   p_vehicle_data->host_speed = cals.k_lcda_host_activation_speed_min - cals.k_lcda_host_activation_speed_min_hys + EPSILON;

   /** \action Call function to set hold time and object id */
   Lcda_Hold_Honda_Alert_Before_Reset_Cvw(&lcda_output, &lcda_core_output, *p_vehicle_data, &cals, &customer_cals);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_CVW_LEFT], -LCDA_LKA_NA_TTC);
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_CVW_RIGHT], -LCDA_LKA_NA_TTC);
}

/**
 * Check if hold times are set correctly.
 * \uts{CSCSA-109133} \sdd{SF-7000} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Honda_Alert_Before_Reset_Cvw__core_alert_on)
{
   /** \arrange Set speed and position */
   Lcda_Post_Run_Init();
   lcda_output.hold_time[HONDA_SIDE_CVW_LEFT]                = -LCDA_LKA_NA_TTC;
   lcda_output.predicted_exit_time[HONDA_SIDE_CVW_LEFT]      = 5.0;
   lcda_output.cvw_alert_left                                = 1u;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT] = LCDA_ALERT_STATE_LEVEL_1;
   lcda_output.hold_obj_index[HONDA_SIDE_CVW_LEFT]           = 2u;
   data.object_data[2u].status                               = PA_OBJ_STATUS_INVALID;

   lcda_output.hold_time[HONDA_SIDE_CVW_RIGHT]                = -LCDA_LKA_NA_TTC;
   lcda_output.predicted_exit_time[HONDA_SIDE_CVW_RIGHT]      = 6.0;
   lcda_output.cvw_alert_right                                = 1u;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_1;
   lcda_output.hold_obj_index[HONDA_SIDE_CVW_RIGHT]           = 3u;
   data.object_data[3u].status                                = PA_OBJ_STATUS_INVALID;

   p_vehicle_data->host_speed = cals.k_lcda_host_activation_speed_min - cals.k_lcda_host_activation_speed_min_hys - EPSILON;

   /** \action Call function to set hold time and object id */
   Lcda_Hold_Honda_Alert_Before_Reset_Cvw(&lcda_output, &lcda_core_output, *p_vehicle_data, &cals, &customer_cals);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_CVW_LEFT], -LCDA_LKA_NA_TTC);
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_CVW_RIGHT], -LCDA_LKA_NA_TTC);
}

/**
 * Check if hold times are not set if calibration is turned off.
 * \uts{CSCSA-109134} \sdd{SF-7001} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Honda_Alert_Before_Reset_Bsw__calibration_disable)
{
   /** \arrange Set speed and position */
   Lcda_Post_Run_Init();

   customer_cals.k_honda_srr6_enable_alert_hold_due_slow_down = FBK_FALSE;

   /** \action Call function to set hold time and object id */
   Lcda_Hold_Honda_Alert_Before_Reset_Bsw(&lcda_output, &lcda_core_output, *p_vehicle_data, &cals, &customer_cals, &data);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.hold_obj_index[HONDA_SIDE_BSW_LEFT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(lcda_output.hold_obj_index[HONDA_SIDE_BSW_RIGHT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_BSW_LEFT], -LCDA_LKA_NA_TTC);
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_BSW_RIGHT], -LCDA_LKA_NA_TTC);
}

/**
 * Check if hold times are not set if calibration is turned off.
 * \uts{CSCSA-109135} \sdd{SF-7000} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Honda_Alert_Before_Reset_Cvw__calibration_disable)
{
   /** \arrange Set speed and position */
   Lcda_Post_Run_Init();
   customer_cals.k_honda_srr6_enable_alert_hold_due_slow_down = FBK_FALSE;

   /** \action Call function to set hold time and object id */
   Lcda_Hold_Honda_Alert_Before_Reset_Cvw(&lcda_output, &lcda_core_output, *p_vehicle_data, &cals, &customer_cals);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.hold_obj_index[HONDA_SIDE_CVW_LEFT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(lcda_output.hold_obj_index[HONDA_SIDE_CVW_RIGHT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_CVW_LEFT], -LCDA_LKA_NA_TTC);
   EXPECT_EQ(lcda_output.hold_time[HONDA_SIDE_CVW_RIGHT], -LCDA_LKA_NA_TTC);
}

/**
 * Check data is correctly set while hold mode is on.
 * \uts{CSCSA-109136} \sdd{SF-6838} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run__check_holding_alert)
{
   /** \arrange Set up LCDA core output with positive hold times and lka object id filled */

   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_LEVEL_2;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_2;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_1;

   Lcda_Get_Lcda_Honda_Instance()->customer_output.LKA_Object_Left[0].lka_obj_id  = 1u;
   Lcda_Get_Lcda_Honda_Instance()->customer_output.LKA_Object_Left[1].lka_obj_id  = 2u;
   Lcda_Get_Lcda_Honda_Instance()->customer_output.LKA_Object_Right[0].lka_obj_id = 3u;
   Lcda_Get_Lcda_Honda_Instance()->customer_output.LKA_Object_Right[1].lka_obj_id = 4u;

   Lcda_Get_Lcda_Honda_Instance()->hold_obj_index[HONDA_SIDE_BSW_LEFT]  = 1u;
   Lcda_Get_Lcda_Honda_Instance()->hold_obj_index[HONDA_SIDE_CVW_LEFT]  = 2u;
   Lcda_Get_Lcda_Honda_Instance()->hold_obj_index[HONDA_SIDE_BSW_RIGHT] = 3u;
   Lcda_Get_Lcda_Honda_Instance()->hold_obj_index[HONDA_SIDE_CVW_RIGHT] = 4u;

   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_LEFT]  = 1u;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_LEFT]  = 2u;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_RIGHT] = 3u;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_RIGHT] = 4u;

   Lcda_Get_Lcda_Honda_Instance()->hold_time[HONDA_SIDE_BSW_LEFT]         = 2.0f;
   Lcda_Get_Lcda_Honda_Instance()->hold_time[HONDA_SIDE_CVW_LEFT]         = 3.0f;
   Lcda_Get_Lcda_Honda_Instance()->hold_time[HONDA_SIDE_BSW_RIGHT]        = 1.0f;
   Lcda_Get_Lcda_Honda_Instance()->hold_time[HONDA_SIDE_CVW_RIGHT]        = 4.0f;
   Lcda_Get_Lcda_Honda_Instance()->hold_alert_level[HONDA_SIDE_BSW_LEFT]  = 2u;
   Lcda_Get_Lcda_Honda_Instance()->hold_alert_level[HONDA_SIDE_CVW_LEFT]  = 1u;
   Lcda_Get_Lcda_Honda_Instance()->hold_alert_level[HONDA_SIDE_BSW_RIGHT] = 2u;
   Lcda_Get_Lcda_Honda_Instance()->hold_alert_level[HONDA_SIDE_CVW_RIGHT] = 1u;
   Lcda_Get_Lcda_Honda_Instance()->hold_object[HONDA_SIDE_BSW_LEFT] =
      Lcda_Get_Lcda_Honda_Instance()->customer_output.LKA_Object_Left[0];
   Lcda_Get_Lcda_Honda_Instance()->hold_object[HONDA_SIDE_CVW_LEFT] =
      Lcda_Get_Lcda_Honda_Instance()->customer_output.LKA_Object_Left[1];
   Lcda_Get_Lcda_Honda_Instance()->hold_object[HONDA_SIDE_BSW_RIGHT] =
      Lcda_Get_Lcda_Honda_Instance()->customer_output.LKA_Object_Right[0];
   Lcda_Get_Lcda_Honda_Instance()->hold_object[HONDA_SIDE_CVW_RIGHT] =
      Lcda_Get_Lcda_Honda_Instance()->customer_output.LKA_Object_Right[1];

   customer_cals.k_honda_ego_speed_stop_holding = 0.833f;
   p_vehicle_data->host_speed                   = customer_cals.k_honda_ego_speed_stop_holding + EPSILON;

   customer_cals.k_honda_srr6_enable_alert_hold_due_slow_down = FBK_TRUE;

   /** \action Call Lcda_Post_Run to fill LCDA output from core output. */
   Lcda_Post_Run(&lcda_instance, &lcda_input, &lcda_output, &fbk_output);

   /** \assert Check that object IDs and alerts are set correctly for all active alerts. */
   EXPECT_EQ(lcda_output.bsw_id_left, 1u);
   EXPECT_EQ(lcda_output.cvw_id_left, 2u);
   EXPECT_EQ(lcda_output.bsw_id_right, 3u);
   EXPECT_EQ(lcda_output.cvw_id_right, 4u);
   EXPECT_EQ(lcda_output.bsw_alert_left, 2u);
   EXPECT_EQ(lcda_output.cvw_alert_left, 1u);
   EXPECT_EQ(lcda_output.bsw_alert_right, 2u);
   EXPECT_EQ(lcda_output.cvw_alert_right, 1u);
}

/**
 * Check if lcda enable flag is true if ego slows down while alert is ON on right side
 * \uts{CSCSA-109137} \sdd{SF-6838} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run__lcda_enable_flag_while_slow_down_right)
{
   /** \arrange Set up LCDA core output with positive hold times */

   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT] = LCDA_ALERT_STATE_NONE;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT] = LCDA_ALERT_STATE_NONE;

   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_1;


   lcda_core_output.lcda_status = LCDA_STATUS_DEACTIVATED_LOW_EGO_SPEED;

   lcda_output.hold_time[HONDA_SIDE_BSW_LEFT]  = 2.0f;
   lcda_output.hold_time[HONDA_SIDE_CVW_LEFT]  = 3.0f;
   lcda_output.hold_time[HONDA_SIDE_BSW_RIGHT] = 1.0f;
   lcda_output.hold_time[HONDA_SIDE_CVW_RIGHT] = 4.0f;

   /** \action Call Lcda_Post_Run to fill LCDA output from core output. */
   Lcda_Post_Run(&lcda_instance, &lcda_input, &lcda_output, &fbk_output);
   ;

   /** \assert Check that enable flag is true. */
   EXPECT_EQ(lcda_output.f_lcda_enabled, 1u);
}

/**
 * Check if lcda enable flag is true if ego slows down while alert is on on left side
 * \uts{CSCSA-109138} \sdd{SF-6838} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run__lcda_enable_flag_while_slow_down_left)
{
   /** \arrange Set up LCDA core output with positive hold times and lka object id filled on left side */

   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_NONE;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;


   lcda_core_output.lcda_status = LCDA_STATUS_DEACTIVATED_LOW_EGO_SPEED;

   lcda_output.hold_time[HONDA_SIDE_BSW_LEFT]  = 2.0f;
   lcda_output.hold_time[HONDA_SIDE_CVW_LEFT]  = 3.0f;
   lcda_output.hold_time[HONDA_SIDE_BSW_RIGHT] = 1.0f;
   lcda_output.hold_time[HONDA_SIDE_CVW_RIGHT] = 4.0f;

   /** \action Call Lcda_Post_Run to fill LCDA output from core output. */
   Lcda_Post_Run(&lcda_instance, &lcda_input, &lcda_output, &fbk_output);
   ;

   /** \assert Check that enable flag is true. */
   EXPECT_EQ(lcda_output.f_lcda_enabled, 1u);
}

/**
 * Check if lcda enable flag is true if ego slows down while alert is on on both sides
 * \uts{CSCSA-109139} \sdd{SF-6838} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run__lcda_enable_flag_while_slow_down_both)
{
   /** \arrange Set up LCDA core output with positive hold times and lka object id filled on both side */

   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_NONE;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;


   lcda_core_output.lcda_status = LCDA_STATUS_DEACTIVATED_LOW_EGO_SPEED;

   lcda_output.hold_time[HONDA_SIDE_BSW_LEFT]  = 2.0f;
   lcda_output.hold_time[HONDA_SIDE_CVW_LEFT]  = 3.0f;
   lcda_output.hold_time[HONDA_SIDE_BSW_RIGHT] = 1.0f;
   lcda_output.hold_time[HONDA_SIDE_CVW_RIGHT] = 4.0f;

   /** \action Call Lcda_Post_Run to fill LCDA output from core output. */
   Lcda_Post_Run(&lcda_instance, &lcda_input, &lcda_output, &fbk_output);
   ;

   /** \assert Check that enable flag is true. */
   EXPECT_EQ(lcda_output.f_lcda_enabled, 1u);
}

/**
 * Check if lcda enable flag is false if ego slows down while alert is off on both sides
 * \uts{CSCSA-109140} \sdd{SF-6838} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Post_Run__lcda_enable_flag_while_slow_down_none)
{
   /** \arrange Set up LCDA core output with positive hold times off alerts */

   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_NONE;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_NONE;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_NONE;


   lcda_core_output.lcda_status = LCDA_STATUS_DEACTIVATED_LOW_EGO_SPEED;

   lcda_output.hold_time[HONDA_SIDE_BSW_LEFT]  = 2.0f;
   lcda_output.hold_time[HONDA_SIDE_CVW_LEFT]  = 3.0f;
   lcda_output.hold_time[HONDA_SIDE_BSW_RIGHT] = 1.0f;
   lcda_output.hold_time[HONDA_SIDE_CVW_RIGHT] = 4.0f;

   /** \action Call Lcda_Post_Run to fill LCDA output from core output. */
   Lcda_Post_Run(&lcda_instance, &lcda_input, &lcda_output, &fbk_output);
   ;

   /** \assert Check that enable flag is true. */
   EXPECT_EQ(lcda_output.f_lcda_enabled, 0u);
}

/**
 * Check if pa objects are translated correctly to honda object
 * \uts{CSCSA-109141} \sdd{SF-6941} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Lka_Get_Object_Class__test_all)
{
   /** \arrange Declare objects */
   HONDA_OBJECT_CLASS_T unknown;
   HONDA_OBJECT_CLASS_T car;
   HONDA_OBJECT_CLASS_T truck;
   HONDA_OBJECT_CLASS_T motorcycle;
   HONDA_OBJECT_CLASS_T pedestrian;
   HONDA_OBJECT_CLASS_T other;

   /** \action Translate pa objects to honda objects */
   unknown    = Lcda_Lka_Get_Object_Class(PA_OBJ_CLASS_UNKNOWN);
   car        = Lcda_Lka_Get_Object_Class(PA_OBJ_CLASS_CAR);
   truck      = Lcda_Lka_Get_Object_Class(PA_OBJ_CLASS_TRUCK);
   motorcycle = Lcda_Lka_Get_Object_Class(PA_OBJ_CLASS_2WHEEL);
   pedestrian = Lcda_Lka_Get_Object_Class(PA_OBJ_CLASS_PEDESTRIAN);
   other      = Lcda_Lka_Get_Object_Class((Pa_Obj_Class_T) 10u);

   /** \assert Check if results are correct */
   EXPECT_EQ(unknown, HONDA_OBJECT_CLASS_UNKNOWN);
   EXPECT_EQ(car, HONDA_OBJECT_CLASS_CAR);
   EXPECT_EQ(truck, HONDA_OBJECT_CLASS_TRUCK);
   EXPECT_EQ(motorcycle, HONDA_OBJECT_CLASS_MOTORCYCLE);
   EXPECT_EQ(pedestrian, HONDA_OBJECT_CLASS_PEDESTRIAN);
   EXPECT_EQ(other, HONDA_OBJECT_CLASS_OTHER);
}

/**
 * Check if pa change status is translated correctly to honda change status
 * \uts{CSCSA-109142} \sdd{SF-6940} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Lka_Get_Change_Status__test_all)
{
   /** \arrange Declare objects */
   HONDA_CHANGE_STATUS_T no_change;
   HONDA_CHANGE_STATUS_T change;

   /** \action Run function */
   no_change = Lcda_Lka_Get_Change_Status(PA_OBJ_STATUS_INVALID);
   change    = Lcda_Lka_Get_Change_Status(PA_OBJ_STATUS_NEW);

   /** \assert Check results */
   EXPECT_EQ(no_change, HONDA_CHANGE_STATUS_NO_CHANGE);
   EXPECT_EQ(change, HONDA_CHANGE_STATUS_CHANGE);
}

/**
 * Check if pa obj status is translated correctly to honda motion class
 * \uts{CSCSA-109143} \sdd{SF-6939} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Lka_Get_Motion_Class__test_all)
{
   /** \arrange Declare objects */
   HONDA_MOTION_CLASS_T unknown;
   HONDA_MOTION_CLASS_T moving;

   /** \action Run function */
   moving  = Lcda_Lka_Get_Motion_Class(PA_OBJ_STATUS_INVALID);
   unknown = Lcda_Lka_Get_Motion_Class(PA_OBJ_STATUS_COASTED);

   /** \assert Check results */
   EXPECT_EQ(moving, HONDA_MOTION_CLASS_MOVING_OBJECT);
   EXPECT_EQ(unknown, HONDA_MOTION_CLASS_UNKNOWN);
}

/**
 * Check if alert condition is calculated correctly
 * \uts{CSCSA-109144} \sdd{SF-6938} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Lka_Get_Alert_Condition__test_all)
{
   /** \arrange Declare objects */
   HONDA_ALERT_CONDITION_T tos;
   HONDA_ALERT_CONDITION_T sot;

   /** \action Run function */
   tos = Lcda_Lka_Get_Alert_Condition(10.0f);
   sot = Lcda_Lka_Get_Alert_Condition(-10.0f);

   /** \assert Check results */
   EXPECT_EQ(tos, HONDA_ALERT_CONDITION_TOS);
   EXPECT_EQ(sot, HONDA_ALERT_CONDITION_SOT);
}

/**
 * Check if honda alert state is correctly set.
 * \uts{CSCSA-109145} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Honda_Alert_State__alert_cvw_no_alert_bsw)
{
   /** \arrange */
   lcda_output.bsw_alert_right = LCDA_ALERT_STATE_NONE;
   lcda_output.cvw_alert_right = LCDA_ALERT_STATE_LEVEL_1;
   lcda_output.bsw_alert_left  = LCDA_ALERT_STATE_NONE;
   lcda_output.cvw_alert_left  = LCDA_ALERT_STATE_LEVEL_1;

   /** \action */
   Lcda_Set_Honda_Alert_State(&lcda_output, &lcda_input, &data, &cals, &customer_cals, &lcda_core_output);

   /** \assert */
   EXPECT_EQ(lcda_output.honda_alert_state[FBK_SIDE_LEFT], HONDA_ALERT_STATE_LEVEL1);
   EXPECT_EQ(lcda_output.honda_alert_state[FBK_SIDE_RIGHT], HONDA_ALERT_STATE_LEVEL1);
}

/**
 * Check if honda alert state is correctly set.
 * \uts{CSCSA-109146} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Honda_Alert_State__Alert_Level_Two)
{
   /** \arrange */
   lcda_output.bsw_alert_right = LCDA_ALERT_STATE_NONE;
   lcda_output.cvw_alert_right = LCDA_ALERT_STATE_LEVEL_2;
   lcda_output.bsw_alert_left  = LCDA_ALERT_STATE_NONE;
   lcda_output.cvw_alert_left  = LCDA_ALERT_STATE_LEVEL_2;

   /** \action */
   Lcda_Set_Honda_Alert_State(&lcda_output, &lcda_input, &data, &cals, &customer_cals, &lcda_core_output);

   /** \assert */
   EXPECT_EQ(lcda_output.honda_alert_state[FBK_SIDE_LEFT], HONDA_ALERT_STATE_LEVEL4);
   EXPECT_EQ(lcda_output.honda_alert_state[FBK_SIDE_RIGHT], HONDA_ALERT_STATE_LEVEL4);
}

/**
 * Check if honda alert state 4 is set due to the beeper zone being disabled due to the connected trailer.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Honda_Alert_State__object_with_trailer_in_Beeper_Zone)
{
   /** \arrange set object with trailer in beeper zone */
   lcda_input.f_beeper_zone                                  = FBK_TRUE;
   lcda_input.f_trailer_present                              = FBK_TRUE;
   lcda_output.bsw_alert_left                                = LCDA_ALERT_STATE_LEVEL_2;
   lcda_core_output.bsw_core_output.bsw_index[FBK_SIDE_LEFT] = 7u;
   object_data[7u].vcs_pos.x     = cals.k_bsw_fixed_zone_x[FRONT_EGO_SIDE] - Fbk_Half(customer_cals.k_honda_beeper_zone_length);
   object_data[7u].vcs_pos.y     = -cals.k_bsw_fixed_zone_y[FRONT_EGO_SIDE] - Fbk_Half(customer_cals.k_honda_beeper_zone_width);
   object_data[7u].length        = 4.75f;
   object_data[7u].width         = 1.79f;
   object_data[7u].vcs_heading   = 0.002f;
   data.vehicle_data.turn_signal = 1u;

   /** \action set honda alert state */
   Lcda_Set_Honda_Alert_State(&lcda_output, &lcda_input, &data, &cals, &customer_cals, &lcda_core_output);

   /** \assert Check if alert is level 4 */
   EXPECT_EQ(lcda_output.honda_alert_state[FBK_SIDE_LEFT], HONDA_ALERT_STATE_LEVEL4);
}

/**
 * Check if honda alert state 4 is set due to the beeper zone being disabled due to input signal set to fasle .
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Honda_Alert_State__Beeper_Zone_signal_disabled)
{
   /** \arrange set object in beeper zone but switch is disabled */
   lcda_input.f_beeper_zone                                  = FBK_FALSE;
   lcda_input.f_trailer_present                              = FBK_FALSE;
   lcda_output.bsw_alert_left                                = LCDA_ALERT_STATE_LEVEL_2;
   lcda_core_output.bsw_core_output.bsw_index[FBK_SIDE_LEFT] = 7u;
   object_data[7u].vcs_pos.x     = cals.k_bsw_fixed_zone_x[FRONT_EGO_SIDE] - Fbk_Half(customer_cals.k_honda_beeper_zone_length);
   object_data[7u].vcs_pos.y     = -cals.k_bsw_fixed_zone_y[FRONT_EGO_SIDE] - Fbk_Half(customer_cals.k_honda_beeper_zone_width);
   object_data[7u].length        = 4.75f;
   object_data[7u].width         = 1.79f;
   object_data[7u].vcs_heading   = 0.002f;
   data.vehicle_data.turn_signal = 1u;

   /** \action set honda alert state */
   Lcda_Set_Honda_Alert_State(&lcda_output, &lcda_input, &data, &cals, &customer_cals, &lcda_core_output);

   /** \assert Check if alert is level 4 */
   EXPECT_EQ(lcda_output.honda_alert_state[FBK_SIDE_LEFT], HONDA_ALERT_STATE_LEVEL4);
}

/**
 * Check if honda alert state is correctly set.
 * \uts{CSCSA-109147} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Honda_Alert_State__alert_level1_both_cvw_bsw)
{
   /** \arrange */
   lcda_input.f_slide_through = FBK_TRUE;
   Lcda_Post_Run_Init();
   lcda_output.bsw_alert_right = LCDA_ALERT_STATE_LEVEL_1;
   lcda_output.cvw_alert_right = LCDA_ALERT_STATE_LEVEL_1;
   lcda_output.bsw_alert_left  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_output.cvw_alert_left  = LCDA_ALERT_STATE_LEVEL_1;

   lcda_output.bsw_id_right        = 1u;
   lcda_output.cvw_id_right        = 2u;
   lcda_output.bsw_id_left         = 3u;
   lcda_output.cvw_id_left         = 4u;
   p_vehicle_data->host_length     = 3.0f;
   data.object_data[0].curvi_pos.x = -p_vehicle_data->host_length;
   data.object_data[1].curvi_pos.x = -p_vehicle_data->host_length;
   data.object_data[2].curvi_pos.x = -p_vehicle_data->host_length;
   data.object_data[3].curvi_pos.x = -p_vehicle_data->host_length;

   data.object_data[0].curvi_pos.y = -5.0f;
   data.object_data[1].curvi_pos.y = 5.0f;
   data.object_data[2].curvi_pos.y = -2.0f;
   data.object_data[3].curvi_pos.y = 2.0f;

   data.object_data[0].curvi_vel_rel.x = 10.f;
   data.object_data[1].curvi_vel_rel.x = 10.f;
   data.object_data[2].curvi_vel_rel.x = 20.f;
   data.object_data[3].curvi_vel_rel.x = 20.f;

   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_1;

   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_LEFT]  = 1u;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_RIGHT] = 2u;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_LEFT]  = 3u;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_RIGHT] = 4u;

   lcda_core_output.bsw_core_output.bsw_index[FBK_SIDE_LEFT]  = 0u;
   lcda_core_output.bsw_core_output.bsw_index[FBK_SIDE_RIGHT] = 1u;
   lcda_core_output.cvw_core_output.cvw_index[FBK_SIDE_LEFT]  = 2u;
   lcda_core_output.cvw_core_output.cvw_index[FBK_SIDE_RIGHT] = 3u;


   customer_cals.k_honda_min_relative_speed_for_alert_level_two                   = 0.0f;
   customer_cals.k_honda_is_slide_through_zone_considered_for_cvw_alert_level_two = FBK_FALSE;

   /** \action */
   Lcda_Set_Honda_Alert_State(&lcda_output, &lcda_input, &data, &cals, &customer_cals, &lcda_core_output);

   /** \assert */
   EXPECT_EQ(lcda_output.honda_alert_state[FBK_SIDE_LEFT], HONDA_ALERT_STATE_LEVEL2);
   EXPECT_EQ(lcda_output.honda_alert_state[FBK_SIDE_RIGHT], HONDA_ALERT_STATE_LEVEL2);
}

/**
 * Check if honda alert state is correctly set.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Honda_Alert_State__alert_level1_both_cvw_bsw_slide_through_disabled)
{
   /** \arrange */
   lcda_input.f_slide_through = FBK_FALSE;
   Lcda_Post_Run_Init();
   lcda_output.bsw_alert_right = LCDA_ALERT_STATE_LEVEL_1;
   lcda_output.cvw_alert_right = LCDA_ALERT_STATE_LEVEL_1;
   lcda_output.bsw_alert_left  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_output.cvw_alert_left  = LCDA_ALERT_STATE_LEVEL_1;

   lcda_output.bsw_id_right        = 1u;
   lcda_output.cvw_id_right        = 2u;
   lcda_output.bsw_id_left         = 3u;
   lcda_output.cvw_id_left         = 4u;
   p_vehicle_data->host_length     = 3.0f;
   data.object_data[0].curvi_pos.x = -p_vehicle_data->host_length;
   data.object_data[1].curvi_pos.x = -p_vehicle_data->host_length;
   data.object_data[2].curvi_pos.x = -p_vehicle_data->host_length;
   data.object_data[3].curvi_pos.x = -p_vehicle_data->host_length;

   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT]  = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_1;

   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_LEFT]  = 1u;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_RIGHT] = 2u;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_LEFT]  = 3u;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_RIGHT] = 4u;

   lcda_core_output.bsw_core_output.bsw_index[FBK_SIDE_LEFT]  = 0u;
   lcda_core_output.bsw_core_output.bsw_index[FBK_SIDE_RIGHT] = 1u;
   lcda_core_output.cvw_core_output.cvw_index[FBK_SIDE_LEFT]  = 2u;
   lcda_core_output.cvw_core_output.cvw_index[FBK_SIDE_RIGHT] = 3u;

   lcda_output.customer_output.LKA_Object_Left[1].lka_obj_id         = 1u;
   lcda_output.customer_output.LKA_Object_Left[0].lka_obj_id         = 2u;
   lcda_output.customer_output.LKA_Object_Left[1].lka_curvi_pos_lat  = -4.0f;
   lcda_output.customer_output.LKA_Object_Left[0].lka_curvi_pos_lat  = -5.0f;
   lcda_output.customer_output.LKA_Object_Left[1].lka_curvi_vel_long = -10.0f;
   lcda_output.customer_output.LKA_Object_Left[0].lka_curvi_vel_long = -8.0f;

   lcda_output.customer_output.LKA_Object_Right[1].lka_obj_id         = 3u;
   lcda_output.customer_output.LKA_Object_Right[0].lka_obj_id         = 4u;
   lcda_output.customer_output.LKA_Object_Right[1].lka_curvi_pos_lat  = -4.0f;
   lcda_output.customer_output.LKA_Object_Right[0].lka_curvi_pos_lat  = -5.0f;
   lcda_output.customer_output.LKA_Object_Right[1].lka_curvi_vel_long = -10.0f;
   lcda_output.customer_output.LKA_Object_Right[0].lka_curvi_vel_long = -8.0f;

   customer_cals.k_honda_min_relative_speed_for_alert_level_two                   = 0.0f;
   customer_cals.k_honda_is_slide_through_zone_considered_for_cvw_alert_level_two = FBK_FALSE;

   /** \action */
   Lcda_Set_Honda_Alert_State(&lcda_output, &lcda_input, &data, &cals, &customer_cals, &lcda_core_output);

   /** \assert */
   EXPECT_EQ(lcda_output.honda_alert_state[FBK_SIDE_LEFT], HONDA_ALERT_STATE_LEVEL1);
   EXPECT_EQ(lcda_output.honda_alert_state[FBK_SIDE_RIGHT], HONDA_ALERT_STATE_LEVEL1);
}

/**
 * Check if the level 2 alert is set correctly.
 * \uts{CSCSA-109148} \sdd{SF-6999} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Should_Alert_Level_Two_Be_Turned_On__negative_positions_and_velocities)
{
   /** \arrange Set LKA objectss */
   uint8_t bsw_idx = 0u;
   uint8_t cvw_idx = 1u;

   data.object_data[cvw_idx].id              = bsw_idx + 1;
   data.object_data[bsw_idx].id              = cvw_idx + 1;
   data.object_data[cvw_idx].curvi_pos.y     = -4.0f;
   data.object_data[bsw_idx].curvi_pos.y     = -5.0f;
   data.object_data[cvw_idx].curvi_vel_rel.x = -10.0f;
   data.object_data[bsw_idx].curvi_vel_rel.x =
      data.object_data[cvw_idx].curvi_vel_rel.x + customer_cals.k_honda_min_relative_speed_for_alert_level_two + EPSILON;
   p_vehicle_data->host_length           = 3.0f;
   data.object_data[bsw_idx].curvi_pos.x = data.object_data[bsw_idx].curvi_pos.y - p_vehicle_data->host_length;
   data.object_data[cvw_idx].curvi_pos.x = data.object_data[cvw_idx].curvi_pos.y - p_vehicle_data->host_length;

   customer_cals.k_honda_is_slide_through_zone_considered_for_cvw_alert_level_two = FBK_FALSE;

   /** \action Call function to check level 2 alert */
   boolean_T result = Lcda_Should_Alert_Level_Two_Be_Turned_On(&data, &customer_cals, cvw_idx, bsw_idx);

   /** \assert Check result */
   EXPECT_TRUE(result);
}

/**
 * Check if the level 2 alert is set correctly.
 * \uts{CSCSA-109149} \sdd{SF-6999} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Should_Alert_Level_Two_Be_Turned_On__negative_positions_and_velocities_rel_speed_below_limit)
{
   /** \arrange Set LKA objectss */
   uint8_t bsw_idx                           = 0u;
   uint8_t cvw_idx                           = 1u;
   data.object_data[cvw_idx].id              = bsw_idx + 1;
   data.object_data[bsw_idx].id              = cvw_idx + 1;
   data.object_data[cvw_idx].curvi_pos.y     = -4.0f;
   data.object_data[bsw_idx].curvi_pos.y     = -5.0f;
   data.object_data[cvw_idx].curvi_vel_rel.x = -10.0f;
   data.object_data[bsw_idx].curvi_vel_rel.x = -8.0f;
   p_vehicle_data->host_length               = 3.0f;
   data.object_data[bsw_idx].curvi_pos.x     = data.object_data[bsw_idx].curvi_pos.y - p_vehicle_data->host_length;
   data.object_data[cvw_idx].curvi_pos.x     = data.object_data[cvw_idx].curvi_pos.y - p_vehicle_data->host_length * 2.0f;

   customer_cals.k_honda_min_relative_speed_for_alert_level_two                   = 5.0f;
   customer_cals.k_honda_is_slide_through_zone_considered_for_cvw_alert_level_two = FBK_FALSE;

   /** \action Call function to check level 2 alert */
   boolean_T result = Lcda_Should_Alert_Level_Two_Be_Turned_On(&data, &customer_cals, cvw_idx, bsw_idx);

   /** \assert Check result */
   EXPECT_FALSE(result);
}

/**
 * Check if the level 2 alert is set correctly.
 * \uts{CSCSA-109150} \sdd{SF-6999} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Should_Alert_Level_Two_Be_Turned_On__object_in_slide_through_zone)
{
   /** \arrange Set LKA objects */
   uint8_t bsw_idx = 0u;
   uint8_t cvw_idx = 1u;

   data.object_data[cvw_idx].id              = bsw_idx + 1;
   data.object_data[bsw_idx].id              = cvw_idx + 1;
   data.object_data[cvw_idx].curvi_pos.y     = 1.5f;
   data.object_data[bsw_idx].curvi_pos.y     = 6.5f;
   data.object_data[cvw_idx].curvi_vel_rel.x = 10.0f;
   data.object_data[bsw_idx].curvi_vel_rel.x =
      data.object_data[cvw_idx].curvi_vel_rel.x - customer_cals.k_honda_min_relative_speed_for_alert_level_two - EPSILON;

   p_vehicle_data->host_length           = 3.0f;
   data.object_data[bsw_idx].curvi_pos.x = data.object_data[bsw_idx].curvi_pos.y - p_vehicle_data->host_length;
   data.object_data[cvw_idx].curvi_pos.x = data.object_data[cvw_idx].curvi_pos.y - p_vehicle_data->host_length;

   customer_cals.k_honda_is_slide_through_zone_considered_for_cvw_alert_level_two = FBK_TRUE;

   /** \action Call function to check level 2 alert */
   boolean_T result = Lcda_Should_Alert_Level_Two_Be_Turned_On(&data, &customer_cals, cvw_idx, bsw_idx);

   /** \assert Check result */
   EXPECT_TRUE(result);
}

/**
 * Check if the level 2 alert is set correctly.
 * \uts{CSCSA-109151} \sdd{SF-6999} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Should_Alert_Level_Two_Be_Turned_On__object_out_of_slide_through_zone)
{
   /** \arrange Set LKA objectss */
   uint8_t bsw_idx = 0u;
   uint8_t cvw_idx = 1u;

   data.object_data[cvw_idx].id              = bsw_idx + 1;
   data.object_data[bsw_idx].id              = cvw_idx + 1;
   data.object_data[cvw_idx].curvi_pos.y     = 2.5f;
   data.object_data[bsw_idx].curvi_pos.y     = 2.0f;
   data.object_data[cvw_idx].curvi_vel_rel.x = 10.0f;
   data.object_data[bsw_idx].curvi_vel_rel.x =
      data.object_data[cvw_idx].curvi_vel_rel.x - customer_cals.k_honda_min_relative_speed_for_alert_level_two - EPSILON;

   customer_cals.k_honda_is_slide_through_zone_considered_for_cvw_alert_level_two = FBK_TRUE;

   /** \action Call function to check level 2 alert */
   boolean_T result = Lcda_Should_Alert_Level_Two_Be_Turned_On(&data, &customer_cals, cvw_idx, bsw_idx);

   /** \assert Check result */
   EXPECT_FALSE(result);
}

/**
 * Check if the level 2 alert is set correctly.
 * \uts{CSCSA-109152} \sdd{SF-6999} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Should_Alert_Level_Two_Be_Turned_On__object_out_of_slide_through_zone_right_side)
{
   /** \arrange Set LKA objectss */
   uint8_t bsw_idx = 0u;
   uint8_t cvw_idx = 1u;

   data.object_data[cvw_idx].id              = bsw_idx + 1;
   data.object_data[bsw_idx].id              = cvw_idx + 1;
   data.object_data[cvw_idx].curvi_pos.y     = -2.5f;
   data.object_data[bsw_idx].curvi_pos.y     = -2.0f;
   data.object_data[cvw_idx].curvi_vel_rel.x = 10.0f;
   data.object_data[bsw_idx].curvi_vel_rel.x =
      data.object_data[cvw_idx].curvi_vel_rel.x - customer_cals.k_honda_min_relative_speed_for_alert_level_two - EPSILON;

   customer_cals.k_honda_is_slide_through_zone_considered_for_cvw_alert_level_two = FBK_TRUE;

   /** \action Call function to check level 2 alert */
   boolean_T result = Lcda_Should_Alert_Level_Two_Be_Turned_On(&data, &customer_cals, cvw_idx, bsw_idx);

   /** \assert Check result */
   EXPECT_FALSE(result);
}

/**
 * Check if the core alert is detected correctly.
 * \uts{CSCSA-109153} \sdd{SF-7003} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Is_Cvw_Core_Alert_On__alert_left_side)
{
   /** \arrange Set Core Output objects */
   boolean_T result;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT] = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_index[FBK_SIDE_LEFT] = 2u;

   /** \action Call function */
   result = Lcda_Is_Cvw_Core_Alert_On(&lcda_core_output, FBK_SIDE_LEFT);

   /** \assert Check result */
   EXPECT_TRUE(result);
}

/**
 * Check if the core alert is detected correctly when invalid obj index.
 * \uts{} \sdd{SF-7003} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Is_Cvw_Core_Alert_On__no_alert_left_side)
{
   /** \arrange Set Core Output objects */
   boolean_T result;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT] = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_index[FBK_SIDE_LEFT] = PA_INVALID_OBJ_INDEX;

   /** \action Call function */
   result = Lcda_Is_Cvw_Core_Alert_On(&lcda_core_output, FBK_SIDE_LEFT);

   /** \assert Check result */
   EXPECT_FALSE(result);
}

/**
 * Check if function setting lka objects runs correctly.
 * \uts{CSCSA-109155} \sdd{CSCSA-92289} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Lka_Object__general_test)
{
   /** \arrange bsw and cvw outputs */
   uint8_t idx;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_LEFT]  = 1u;
   lcda_core_output.bsw_core_output.bsw_id[FBK_SIDE_RIGHT] = 2u;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_LEFT]  = 3u;
   lcda_core_output.cvw_core_output.cvw_id[FBK_SIDE_RIGHT] = 4u;
   object_data[0].id                                       = 1u;
   object_data[1].id                                       = 2u;
   object_data[2].id                                       = 3u;
   object_data[3].id                                       = 4u;
   LKA_Object_T lka_objects[HONDA_SIDE_NUMBER];

   /** \action Call function */
   for (idx = 0; idx < HONDA_SIDE_NUMBER; idx++)
   {
      Lcda_Set_Lka_Object(&(lka_objects[idx]), idx, p_vehicle_data, &lcda_core_output, &data, (HONDA_SIDE_T) idx);
   }
   /** \assert Check result */
   for (idx = 0; idx < HONDA_SIDE_NUMBER; idx++)
   {
      EXPECT_EQ(lka_objects[idx].lka_obj_id, idx + 1);
   }
}

/**
 * Check if function setting lka objects runs correctly. Here default case is tested.
 * \uts{CSCSA-109156} \sdd{CSCSA-92289} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Lka_Object__default_test)
{
   /** \arrange bsw and cvw outputs */
   LKA_Object_T lka_object;

   /** \action Call function */
   Lcda_Set_Lka_Object(&lka_object, 0u, p_vehicle_data, &lcda_core_output, &data, (HONDA_SIDE_T) 4u);

   /** \assert Check result */
   EXPECT_EQ(lka_object.lka_obj_id, LCDA_LKA_NA_OBJECT_ID);
}

/**
 * Testing function setting level 2 alert when previus alert is on and cvw object overtakes bsw object.
 * \uts{CSCSA-109157} \sdd{CSCSA-92290} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Process_Slide_Through_Level_2_Alert__previous_alert_on)
{
   /** \arrange bsw and cvw outputs. Cvw closer and faster than bsw */
   uint8_t side                                                = FBK_SIDE_LEFT;
   uint8_t bsw_idx                                             = 0u;
   uint8_t cvw_idx                                             = 1u;
   lcda_core_output.bsw_core_output.bsw_alert[side]            = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[side]            = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.bsw_core_output.bsw_id[side]               = bsw_idx + 1u;
   lcda_core_output.cvw_core_output.cvw_id[side]               = cvw_idx + 1u;
   lcda_core_output.bsw_core_output.bsw_index[side]            = bsw_idx;
   lcda_core_output.cvw_core_output.cvw_index[side]            = cvw_idx;
   lcda_core_output.bsw_core_output.f_obj_in_bsw_zone[bsw_idx] = FBK_TRUE;
   lcda_core_output.bsw_core_output.f_obj_in_bsw_zone[cvw_idx] = FBK_TRUE;

   data.object_data[cvw_idx].id              = bsw_idx + 1u;
   data.object_data[bsw_idx].id              = cvw_idx + 1u;
   data.object_data[cvw_idx].curvi_pos.y     = 1.5f;
   data.object_data[bsw_idx].curvi_pos.y     = 6.5f;
   data.object_data[cvw_idx].curvi_vel_rel.x = 10.0f;
   data.object_data[bsw_idx].curvi_vel_rel.x =
      data.object_data[cvw_idx].curvi_vel_rel.x - customer_cals.k_honda_min_relative_speed_for_alert_level_two - EPSILON;

   p_vehicle_data->host_length           = 3.0f;
   data.object_data[bsw_idx].curvi_pos.x = data.object_data[bsw_idx].curvi_pos.y - p_vehicle_data->host_length;
   data.object_data[cvw_idx].curvi_pos.x = data.object_data[cvw_idx].curvi_pos.y - p_vehicle_data->host_length;

   customer_cals.k_honda_alert_level_two_holding_time                             = 0.0f;
   customer_cals.k_honda_is_slide_through_zone_considered_for_cvw_alert_level_two = FBK_FALSE;

   lcda_input.f_slide_through = FBK_TRUE;

   /** \action Call function */
   /* call several time to set static variables */
   Lcda_Process_Slide_Through_Level_2_Alert(&lcda_output, &lcda_input, &data, &customer_cals, &lcda_core_output);

   data.object_data[bsw_idx].curvi_vel_rel.x =
      data.object_data[cvw_idx].curvi_vel_rel.x - customer_cals.k_honda_min_relative_speed_for_alert_level_two + EPSILON;
   Lcda_Process_Slide_Through_Level_2_Alert(&lcda_output, &lcda_input, &data, &customer_cals, &lcda_core_output);

   data.object_data[bsw_idx].curvi_vel_rel.x =
      data.object_data[cvw_idx].curvi_vel_rel.x - customer_cals.k_honda_min_relative_speed_for_alert_level_two - EPSILON;
   Lcda_Process_Slide_Through_Level_2_Alert(&lcda_output, &lcda_input, &data, &customer_cals, &lcda_core_output);

   /* update output to test overtake case, bsw objcet ==cvw object */
   lcda_core_output.cvw_core_output.cvw_id[side] = lcda_core_output.bsw_core_output.bsw_id[side];

   /* call second time to test hysteresis scenario*/
   Lcda_Process_Slide_Through_Level_2_Alert(&lcda_output, &lcda_input, &data, &customer_cals, &lcda_core_output);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.honda_alert_state[side], HONDA_ALERT_STATE_LEVEL2);
}

/**
 * Testing function setting level 2 alert when previus alert is on and cvw object overtakes bsw object. Here, cvw object has left
 * the zone \uts{CSCSA-109158} \sdd{CSCSA-92290} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Process_Slide_Through_Level_2_Alert__previous_alert_on_cvw_left_zone)
{
   /** \arrange bsw and cvw outputs. Cvw closer and faster than bsw */
   uint8_t side                                                = FBK_SIDE_LEFT;
   uint8_t bsw_idx                                             = 0u;
   uint8_t cvw_idx                                             = 1u;
   lcda_core_output.bsw_core_output.bsw_alert[side]            = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[side]            = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.bsw_core_output.bsw_id[side]               = bsw_idx + 1u;
   lcda_core_output.cvw_core_output.cvw_id[side]               = cvw_idx + 1u;
   lcda_core_output.bsw_core_output.bsw_index[side]            = bsw_idx;
   lcda_core_output.cvw_core_output.cvw_index[side]            = cvw_idx;
   lcda_core_output.bsw_core_output.f_obj_in_bsw_zone[bsw_idx] = FBK_TRUE;
   lcda_core_output.bsw_core_output.f_obj_in_bsw_zone[cvw_idx] = FBK_TRUE;

   data.object_data[cvw_idx].id              = bsw_idx + 1;
   data.object_data[bsw_idx].id              = cvw_idx + 1;
   data.object_data[cvw_idx].curvi_pos.y     = 1.5f;
   data.object_data[bsw_idx].curvi_pos.y     = 6.5f;
   data.object_data[cvw_idx].curvi_vel_rel.x = 10.0f;
   data.object_data[bsw_idx].curvi_vel_rel.x =
      data.object_data[cvw_idx].curvi_vel_rel.x - customer_cals.k_honda_min_relative_speed_for_alert_level_two - EPSILON;

   p_vehicle_data->host_length           = 3.0f;
   data.object_data[bsw_idx].curvi_pos.x = data.object_data[bsw_idx].curvi_pos.y - p_vehicle_data->host_length;
   data.object_data[cvw_idx].curvi_pos.x = data.object_data[cvw_idx].curvi_pos.y - p_vehicle_data->host_length;

   customer_cals.k_honda_is_slide_through_zone_considered_for_cvw_alert_level_two = FBK_TRUE;

   lcda_input.f_slide_through = FBK_TRUE;

   /** \action Call function */
   /* call first and second time to set static variables */
   Lcda_Process_Slide_Through_Level_2_Alert(&lcda_output, &lcda_input, &data, &customer_cals, &lcda_core_output);
   Lcda_Process_Slide_Through_Level_2_Alert(&lcda_output, &lcda_input, &data, &customer_cals, &lcda_core_output);

   /* update output to test overtake case, bsw objcet ==cvw object */
   lcda_core_output.cvw_core_output.cvw_id[side]               = lcda_core_output.bsw_core_output.bsw_id[side];
   lcda_output.honda_alert_state[side]                         = HONDA_ALERT_STATE_LEVEL1;
   lcda_core_output.bsw_core_output.f_obj_in_bsw_zone[cvw_idx] = FBK_FALSE;
   customer_cals.k_honda_alert_level_two_holding_time          = 0.0f;

   /* call second time to test hysteresis scenario*/
   Lcda_Process_Slide_Through_Level_2_Alert(&lcda_output, &lcda_input, &data, &customer_cals, &lcda_core_output);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.honda_alert_state[side], HONDA_ALERT_STATE_LEVEL1);
}

/**
 * Testing function setting level 2 alert when previus alert is on and cvw object overtakes bsw object. Here, bsw object has left
 * the zone \uts{CSCSA-109159} \sdd{CSCSA-92290} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Process_Slide_Through_Level_2_Alert__previous_alert_on_bsw_left_zone)
{
   /** \arrange bsw and cvw outputs. Cvw closer and faster than bsw */
   uint8_t side                                                = FBK_SIDE_LEFT;
   uint8_t bsw_idx                                             = 0u;
   uint8_t cvw_idx                                             = 1u;
   lcda_core_output.bsw_core_output.bsw_alert[side]            = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[side]            = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.bsw_core_output.bsw_id[side]               = bsw_idx + 1u;
   lcda_core_output.cvw_core_output.cvw_id[side]               = cvw_idx + 1u;
   lcda_core_output.bsw_core_output.bsw_index[side]            = bsw_idx;
   lcda_core_output.cvw_core_output.cvw_index[side]            = cvw_idx;
   lcda_core_output.bsw_core_output.f_obj_in_bsw_zone[bsw_idx] = FBK_TRUE;
   lcda_core_output.bsw_core_output.f_obj_in_bsw_zone[cvw_idx] = FBK_TRUE;

   data.object_data[cvw_idx].id              = bsw_idx + 1;
   data.object_data[bsw_idx].id              = cvw_idx + 1;
   data.object_data[cvw_idx].curvi_pos.y     = 1.5f;
   data.object_data[bsw_idx].curvi_pos.y     = 6.5f;
   data.object_data[cvw_idx].curvi_vel_rel.x = 10.0f;
   data.object_data[bsw_idx].curvi_vel_rel.x =
      data.object_data[cvw_idx].curvi_vel_rel.x - customer_cals.k_honda_min_relative_speed_for_alert_level_two - EPSILON;

   p_vehicle_data->host_length           = 3.0f;
   data.object_data[bsw_idx].curvi_pos.x = data.object_data[bsw_idx].curvi_pos.y - p_vehicle_data->host_length;
   data.object_data[cvw_idx].curvi_pos.x = data.object_data[cvw_idx].curvi_pos.y - p_vehicle_data->host_length;

   customer_cals.k_honda_is_slide_through_zone_considered_for_cvw_alert_level_two = FBK_TRUE;

   lcda_input.f_slide_through = FBK_TRUE;

   /** \action Call function */
   /* call first and second time to set static variables */
   Lcda_Process_Slide_Through_Level_2_Alert(&lcda_output, &lcda_input, &data, &customer_cals, &lcda_core_output);
   Lcda_Process_Slide_Through_Level_2_Alert(&lcda_output, &lcda_input, &data, &customer_cals, &lcda_core_output);

   /* update output to test overtake case, bsw objcet ==cvw object */
   lcda_core_output.cvw_core_output.cvw_id[side]               = lcda_core_output.bsw_core_output.bsw_id[side];
   lcda_output.honda_alert_state[side]                         = HONDA_ALERT_STATE_LEVEL1;
   lcda_core_output.bsw_core_output.f_obj_in_bsw_zone[bsw_idx] = FBK_FALSE;
   customer_cals.k_honda_alert_level_two_holding_time          = 0.0f;

   /* call second time to test hysteresis scenario*/
   Lcda_Process_Slide_Through_Level_2_Alert(&lcda_output, &lcda_input, &data, &customer_cals, &lcda_core_output);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.honda_alert_state[side], HONDA_ALERT_STATE_LEVEL1);
}

/**
 * Testing function setting level 2 alert when previus alert is on and cvw object overtakes bsw object. Cvw object crosses ego
 * front bumper \uts{CSCSA-109160} \sdd{CSCSA-92290} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Process_Slide_Through_Level_2_Alert__previous_alert_on_cvw_crosses_ego)
{
   /** \arrange bsw and cvw outputs. Cvw closer and faster than bsw */
   uint8_t side                                                = FBK_SIDE_LEFT;
   uint8_t bsw_idx                                             = 0u;
   uint8_t cvw_idx                                             = 1u;
   lcda_core_output.bsw_core_output.bsw_alert[side]            = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[side]            = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.bsw_core_output.bsw_id[side]               = bsw_idx + 1u;
   lcda_core_output.cvw_core_output.cvw_id[side]               = cvw_idx + 1u;
   lcda_core_output.bsw_core_output.bsw_index[side]            = bsw_idx;
   lcda_core_output.cvw_core_output.cvw_index[side]            = cvw_idx;
   lcda_core_output.bsw_core_output.f_obj_in_bsw_zone[bsw_idx] = FBK_TRUE;
   lcda_core_output.bsw_core_output.f_obj_in_bsw_zone[cvw_idx] = FBK_TRUE;

   data.object_data[cvw_idx].id              = bsw_idx + 1;
   data.object_data[bsw_idx].id              = cvw_idx + 1;
   data.object_data[cvw_idx].curvi_pos.y     = 1.5f;
   data.object_data[bsw_idx].curvi_pos.y     = 6.5f;
   data.object_data[cvw_idx].curvi_vel_rel.x = 10.0f;
   data.object_data[bsw_idx].curvi_vel_rel.x =
      data.object_data[cvw_idx].curvi_vel_rel.x - customer_cals.k_honda_min_relative_speed_for_alert_level_two - EPSILON;

   p_vehicle_data->host_length           = 3.0f;
   data.object_data[bsw_idx].curvi_pos.x = data.object_data[bsw_idx].curvi_pos.y - p_vehicle_data->host_length;
   data.object_data[cvw_idx].curvi_pos.x = data.object_data[cvw_idx].curvi_pos.y - p_vehicle_data->host_length;

   customer_cals.k_honda_is_slide_through_zone_considered_for_cvw_alert_level_two = FBK_TRUE;

   lcda_input.f_slide_through = FBK_TRUE;

   /** \action Call function */
   /* call first time to set static variables */
   Lcda_Process_Slide_Through_Level_2_Alert(&lcda_output, &lcda_input, &data, &customer_cals, &lcda_core_output);


   /* update output to test overtake case, bsw objcet ==cvw object */
   lcda_core_output.cvw_core_output.cvw_id[side]      = lcda_core_output.bsw_core_output.bsw_id[side];
   data.object_data[cvw_idx].curvi_pos.x              = p_vehicle_data->host_length;
   lcda_output.honda_alert_state[side]                = HONDA_ALERT_STATE_LEVEL1;
   customer_cals.k_honda_alert_level_two_holding_time = 0.0f;

   /* call second time to test hysteresis scenario*/
   Lcda_Process_Slide_Through_Level_2_Alert(&lcda_output, &lcda_input, &data, &customer_cals, &lcda_core_output);

   /** \assert Check result */
   EXPECT_EQ(lcda_output.honda_alert_state[side], HONDA_ALERT_STATE_LEVEL1);
}

/**
 * Test that lcda status is set correctly
 * \uts{CSCSA-109161} \sdd{CSCSA-92291} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Get_Feauture_Status__general_test)
{
   /** \arrange core output with enable flags on */
   uint8_t result;
   lcda_core_output.lcda_status = LCDA_STATUS_ACTIVE;

   /** \action Call function */
   result = Lcda_Get_Feauture_Status(&lcda_output, &lcda_core_output);

   /** \assert Check result */
   EXPECT_EQ(result, FBK_ONE_UINT);
}


/**
 * Check if honda alert state 3 is correctly set.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Set_Honda_Alert_State__alert_level_3)
{
   /** \arrange set object in beeper zone */
   lcda_input.f_beeper_zone                                  = FBK_TRUE;
   lcda_input.f_trailer_present                              = FBK_FALSE;
   lcda_output.bsw_alert_left                                = LCDA_ALERT_STATE_LEVEL_2;
   lcda_core_output.bsw_core_output.bsw_index[FBK_SIDE_LEFT] = 7u;
   object_data[7u].vcs_pos.x                                 = cals.k_bsw_x0 - data.vehicle_data.host_length;
   object_data[7u].vcs_pos.y                                 = -Fbk_Half(data.vehicle_data.host_width);
   object_data[7u].length                                    = 4.75f;
   object_data[7u].width                                     = 1.79f;
   object_data[7u].vcs_heading                               = 0.002f;
   data.vehicle_data.turn_signal                             = 1u;
   data.vehicle_data.host_speed                              = 7.0f;
   /** \action set honda alert state */
   Lcda_Set_Honda_Alert_State(&lcda_output, &lcda_input, &data, &cals, &customer_cals, &lcda_core_output);
   /** \assert Check if alert is level 3*/
   EXPECT_EQ(lcda_output.honda_alert_state[FBK_SIDE_LEFT], HONDA_ALERT_STATE_LEVEL3);
}

/**
 * Check if additional controls for beeper logic are set correctly when host speed is above threshold.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Process_Host_Speed_For_Beeper_Logic__speed_above_thresh)
{
   /** \arrange set object in beeper zone */
   boolean_T f_narrow_beeper_active_speed = FBK_TRUE;
   boolean_T f_beeper_active_speed        = FBK_FALSE;

   data.vehicle_data.host_speed = 10.0f;

   /** \action Process host speed for beeper logic */
   Lcda_Process_Host_Speed_For_Beeper_Logic(data.vehicle_data.host_speed, FBK_SIDE_LEFT, &f_narrow_beeper_active_speed,
                                            &f_beeper_active_speed, &cals, &customer_cals);

   /** \assert Check if control flags for beeper logic are correct*/
   EXPECT_EQ(f_narrow_beeper_active_speed, FBK_FALSE);
   EXPECT_EQ(f_beeper_active_speed, FBK_TRUE);
}


/**
 * Check if additional controls for beeper logic are set correctly when host speed is below threshold.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Process_Host_Speed_For_Beeper_Logic__speed_below_thresh)
{
   /** \arrange set object in beeper zone */
   boolean_T f_narrow_beeper_active_speed = FBK_FALSE;
   boolean_T f_beeper_active_speed        = FBK_TRUE;

   data.vehicle_data.host_speed = 0.5f;

   /** \action Process host speed for beeper logic */
   Lcda_Process_Host_Speed_For_Beeper_Logic(data.vehicle_data.host_speed, FBK_SIDE_LEFT, &f_narrow_beeper_active_speed,
                                            &f_beeper_active_speed, &cals, &customer_cals);

   /** \assert Check if control flags for beeper logic are correct*/
   EXPECT_EQ(f_narrow_beeper_active_speed, FBK_TRUE);
   EXPECT_EQ(f_beeper_active_speed, FBK_FALSE);
}


/**
 * Check if additional controls for beeper logic are set correctly when host speed is in range and persist true.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Process_Host_Speed_For_Beeper_Logic__speed_in_range_persist_true)
{
   /** \arrange set object in beeper zone */
   boolean_T f_narrow_beeper_active_speed = FBK_FALSE;
   boolean_T f_beeper_active_speed        = FBK_FALSE;
   uint8_t side                           = FBK_SIDE_LEFT;

   data.vehicle_data.host_speed                                     = 1.5f;
   customer_cals.k_lcda_honda_narrow_beeper_max_speed_l             = 1.0f;
   Lcda_Get_Lcda_Honda_Instance()->f_narrow_beeper_prev_cycle[side] = FBK_TRUE;
   Lcda_Get_Lcda_Honda_Instance()->f_beeper_prev_cycle[side]        = FBK_TRUE;

   /** \action Process host speed for beeper logic */
   Lcda_Process_Host_Speed_For_Beeper_Logic(data.vehicle_data.host_speed, side, &f_narrow_beeper_active_speed,
                                            &f_beeper_active_speed, &cals, &customer_cals);

   /** \assert Check if control flags for beeper logic are correct*/
   EXPECT_EQ(f_narrow_beeper_active_speed, FBK_TRUE);
   EXPECT_EQ(f_beeper_active_speed, FBK_TRUE);
}


/**
 * Check if additional controls for beeper logic are set correctly when host speed is in range and persist false.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Post_Run_Test, Lcda_Process_Host_Speed_For_Beeper_Logic__speed_in_range_persist_false)
{
   /** \arrange set object in beeper zone */
   boolean_T f_narrow_beeper_active_speed = FBK_TRUE;
   boolean_T f_beeper_active_speed        = FBK_TRUE;
   uint8_t side                           = FBK_SIDE_LEFT;

   data.vehicle_data.host_speed                                     = 1.5f;
   customer_cals.k_lcda_honda_narrow_beeper_max_speed_l             = 1.0f;
   Lcda_Get_Lcda_Honda_Instance()->f_narrow_beeper_prev_cycle[side] = FBK_FALSE;
   Lcda_Get_Lcda_Honda_Instance()->f_beeper_prev_cycle[side]        = FBK_FALSE;

   /** \action Process host speed for beeper logic */
   Lcda_Process_Host_Speed_For_Beeper_Logic(data.vehicle_data.host_speed, side, &f_narrow_beeper_active_speed,
                                            &f_beeper_active_speed, &cals, &customer_cals);

   /** \assert Check if control flags for beeper logic are correct*/
   EXPECT_EQ(f_narrow_beeper_active_speed, FBK_FALSE);
   EXPECT_EQ(f_beeper_active_speed, FBK_FALSE);
}


/** Check if object outside beep zone is correctly detected. * \uts{} \sdd{}
\testtype{positive} */
TEST_F(Lcda_Post_Run_Test, Lcda_Is_Obj_In_Narrow_Beeper_Zone__obj_out_zone)
{
   /** \arrange set bsw object inside beeper zone */
   boolean_T f_zone_overlap                         = FBK_TRUE;
   uint8_t side                                     = FBK_SIDE_LEFT;
   lcda_output.honda_alert_state[side]              = HONDA_ALERT_STATE_LEVEL4;
   lcda_core_output.bsw_core_output.bsw_index[side] = 0u;
   object_data->vcs_pos.x      = cals.k_bsw_fixed_zone_x[FRONT_EGO_SIDE] - (10.f * customer_cals.k_honda_beeper_zone_length);
   object_data->vcs_pos.y      = cals.k_bsw_fixed_zone_y[FRONT_EGO_SIDE] + (10.f * customer_cals.k_honda_beeper_zone_width);
   object_data->width          = 1.0f;
   object_data->length         = 2.0f;
   p_vehicle_data->turn_signal = FBK_ONE_UINT;

   /** \action Process object and narrow beeper zone */
   f_zone_overlap = Lcda_Is_Obj_In_Narrow_Beeper_Zone(side, &lcda_output, &data, &cals, &customer_cals, &lcda_core_output);

   /** \assert Check is object is in the zone */
   EXPECT_FALSE(f_zone_overlap);
}


/** Check if object inside beep zone without hysteresis is correctly detected. * \uts{} \sdd{}
\testtype{positive} */
TEST_F(Lcda_Post_Run_Test, Lcda_Is_Obj_In_Narrow_Beeper_Zone__obj_inside_zone_no_hysteresis)
{
   /** \arrange set bsw object inside beeper zone */
   boolean_T f_zone_overlap                         = FBK_FALSE;
   uint8_t side                                     = FBK_SIDE_LEFT;
   lcda_output.honda_alert_state[side]              = HONDA_ALERT_STATE_LEVEL4;
   lcda_core_output.bsw_core_output.bsw_index[side] = 0u;
   object_data->vcs_pos.x                           = -cals.k_bsw_fixed_zone_x[FRONT_EGO_SIDE];
   object_data->vcs_pos.y                           = -cals.k_bsw_fixed_zone_y[FRONT_EGO_SIDE];
   object_data->width                               = 1.0f;
   object_data->length                              = 2.0f;
   p_vehicle_data->turn_signal                      = FBK_ONE_UINT;

   /** \action Process object and narrow beeper zone */
   f_zone_overlap = Lcda_Is_Obj_In_Narrow_Beeper_Zone(side, &lcda_output, &data, &cals, &customer_cals, &lcda_core_output);

   /** \assert Check is object is in the zone */
   EXPECT_TRUE(f_zone_overlap);
}


/** Check if object inside beep zone with hysteresis is correctly detected. * \uts{} \sdd{}
\testtype{positive} */
TEST_F(Lcda_Post_Run_Test, Lcda_Is_Obj_In_Narrow_Beeper_Zone__obj_inside_zone_with_hysteresis)
{
   /** \arrange set bsw object inside beeper zone */
   boolean_T f_zone_overlap                         = FBK_FALSE;
   uint8_t side                                     = FBK_SIDE_LEFT;
   lcda_output.honda_alert_state[side]              = HONDA_ALERT_STATE_LEVEL3;
   lcda_core_output.bsw_core_output.bsw_index[side] = 0u;
   object_data->vcs_pos.x                           = -cals.k_bsw_fixed_zone_x[FRONT_EGO_SIDE];
   object_data->vcs_pos.y                           = -cals.k_bsw_fixed_zone_y[FRONT_EGO_SIDE];
   object_data->width                               = 1.0f;
   object_data->length                              = 2.0f;
   p_vehicle_data->turn_signal                      = FBK_ONE_UINT;

   /** \action Process object and narrow beeper zone */
   f_zone_overlap = Lcda_Is_Obj_In_Narrow_Beeper_Zone(side, &lcda_output, &data, &cals, &customer_cals, &lcda_core_output);

   /** \assert Check is object is in the zone */
   EXPECT_TRUE(f_zone_overlap);
}


/** Check if alert for object out of FOV should be held. * \uts{} \sdd{}
\testtype{positive} */
TEST_F(Lcda_Post_Run_Test, Lcda_Hold_Alert_Obj_Out_Of_Fov__hold_alert)
{
   /** \arrange set bsw object inside beeper zone */
   uint8_t side                                                          = FBK_SIDE_LEFT;
   lcda_output.hold_obj_index[side]                                      = 1u;
   data.object_data[lcda_output.hold_obj_index[side]].status             = PA_OBJ_STATUS_INVALID;
   Lcda_Get_Lcda_Honda_Instance()->f_lcda_level_3_alert_prev_cycle[side] = FBK_TRUE;
   lcda_output.honda_alert_state[side]                                   = HONDA_ALERT_STATE_LEVEL4;

   /** \action Process holding for beeper logic */
   Lcda_Hold_Alert_Obj_Out_Of_Fov(side, &lcda_output, &data, &customer_cals);

   /** \assert Check alert state if it was held */
   EXPECT_EQ(lcda_output.honda_alert_state[side], HONDA_ALERT_STATE_LEVEL3);
   EXPECT_EQ(Lcda_Get_Lcda_Honda_Instance()->f_lcda_level_3_alert_prev_cycle[side], FBK_TRUE);
}
