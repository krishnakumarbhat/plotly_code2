/**
 * @file cta_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for rivian srr6 cta post run
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-71100}
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
 * \uts{CSCSA-71101} \sdd{SF-3954} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Reset_Output__resets_output_properly)
{
   /** \arrange Cta_Post_Run_Init does not have inputs, so no operation is required. */

   /** \action Call Cta_Post_Run_Init, such that cta_output is filled accordingly. */
   Cta_Reset_Output(&(cta_output));

   /** \assert Check that cta_output is filled correctly. */
   EXPECT_FLOAT_EQ(cta_output.cta_obj_ttc_left, CTA_HIGH_DEFAULT_VAL);
   EXPECT_FLOAT_EQ(cta_output.cta_obj_ttc_right, CTA_HIGH_DEFAULT_VAL);
   EXPECT_EQ(cta_output.cta_alert_level_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_alert_level_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_id_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_id_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_warn_hold_cnt_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_warn_hold_cnt_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_brake_hold_cnt_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_brake_hold_cnt_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_brake_supp_cnt_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_brake_supp_cnt_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_brake_qualifier_left, FBK_FALSE);
   EXPECT_EQ(cta_output.f_brake_qualifier_right, FBK_FALSE);
   EXPECT_EQ(cta_output.f_cta_enabled, FBK_FALSE);
   EXPECT_EQ(cta_output.cta_stla_crit_zone_left, CTA_STLA_CRIT_ZONE_NONE);
   EXPECT_EQ(cta_output.cta_stla_crit_zone_right, CTA_STLA_CRIT_ZONE_NONE);
   EXPECT_EQ(cta_output.DBG_Crit_Zone_Right_P0_PositionX, FBK_ZERO_F);
   EXPECT_EQ(cta_output.DBG_Crit_Zone_Right_P0_PositionY, FBK_ZERO_F);
   EXPECT_EQ(cta_output.DBG_Crit_Zone_Right_P1_PositionX, FBK_ZERO_F);
   EXPECT_EQ(cta_output.DBG_Crit_Zone_Right_P1_PositionY, FBK_ZERO_F);
   EXPECT_EQ(cta_output.DBG_Crit_Zone_Right_P2_PositionX, FBK_ZERO_F);
   EXPECT_EQ(cta_output.DBG_Crit_Zone_Right_P2_PositionY, FBK_ZERO_F);
   EXPECT_EQ(cta_output.DBG_Crit_Zone_Right_P3_PositionX, FBK_ZERO_F);
   EXPECT_EQ(cta_output.DBG_Crit_Zone_Right_P3_PositionY, FBK_ZERO_F);
}

/**
 * Check mapping of Cta_Post_Run.
 * \uts{CSCSA-71102} \sdd{SF-3947} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Post_Run__is_filled_properly_from_core_output)
{
   /** \arrange Set up values, which are used in Cta_Post_Run with arbitrary values. */
   float32_T expected_P0_x;
   float32_T expected_P0_y;
   float32_T expected_P1_x;
   float32_T expected_P1_y;
   float32_T expected_P2_x;
   float32_T expected_P2_y;
   float32_T expected_P3_x;
   float32_T expected_P3_y;
   cta_input.f_cta_enable = FBK_TRUE;

   cta_core_output.cta_alert_level[CTA_MODE_FRONT][FBK_SIDE_LEFT]  = CTA_CRIT_LEVEL_1;
   cta_core_output.cta_alert_level[CTA_MODE_FRONT][FBK_SIDE_RIGHT] = CTA_CRIT_LEVEL_2;
   cta_core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT]   = CTA_CRIT_LEVEL_1;
   cta_core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT]  = CTA_CRIT_LEVEL_2;

   cta_core_output.cta_obj_ttc[CTA_MODE_FRONT][FBK_SIDE_LEFT]  = 3.1f;
   cta_core_output.cta_obj_ttc[CTA_MODE_FRONT][FBK_SIDE_RIGHT] = 2.0f;
   cta_core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT]   = 4.1f;
   cta_core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT]  = 7.0f;

   cta_core_output.cta_long_intersection[CTA_MODE_FRONT][FBK_SIDE_LEFT]  = 1.1f;
   cta_core_output.cta_long_intersection[CTA_MODE_FRONT][FBK_SIDE_RIGHT] = 2.2f;
   cta_core_output.cta_long_intersection[CTA_MODE_REAR][FBK_SIDE_LEFT]   = 3.3f;
   cta_core_output.cta_long_intersection[CTA_MODE_REAR][FBK_SIDE_RIGHT]  = 4.4f;

   cta_core_output.cta_heading[CTA_MODE_FRONT][FBK_SIDE_LEFT]  = 11.1f;
   cta_core_output.cta_heading[CTA_MODE_FRONT][FBK_SIDE_RIGHT] = 12.2f;
   cta_core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_LEFT]   = 13.3f;
   cta_core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_RIGHT]  = 14.4f;

   cta_core_output.cta_id[CTA_MODE_FRONT][FBK_SIDE_LEFT]  = 8u;
   cta_core_output.cta_id[CTA_MODE_FRONT][FBK_SIDE_RIGHT] = 12u;
   cta_core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT]   = 14u;
   cta_core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]  = 21u;

   cta_core_output.cta_index[CTA_MODE_FRONT][FBK_SIDE_LEFT]  = 23u;
   cta_core_output.cta_index[CTA_MODE_FRONT][FBK_SIDE_RIGHT] = 3u;
   cta_core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT]   = 6u;
   cta_core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_RIGHT]  = 19u;

   cta_core_output.f_brake_qualifier[CTA_MODE_FRONT][FBK_SIDE_LEFT]  = FBK_FALSE;
   cta_core_output.f_brake_qualifier[CTA_MODE_FRONT][FBK_SIDE_RIGHT] = FBK_TRUE;
   cta_core_output.f_brake_qualifier[CTA_MODE_REAR][FBK_SIDE_LEFT]   = FBK_FALSE;
   cta_core_output.f_brake_qualifier[CTA_MODE_REAR][FBK_SIDE_RIGHT]  = FBK_TRUE;

   cta_core_output.cta_warn_hold_cnt[CTA_MODE_FRONT][FBK_SIDE_LEFT]   = 23u;
   cta_core_output.cta_warn_hold_cnt[CTA_MODE_FRONT][FBK_SIDE_RIGHT]  = 57u;
   cta_core_output.cta_warn_hold_cnt[CTA_MODE_REAR][FBK_SIDE_LEFT]    = 1u;
   cta_core_output.cta_warn_hold_cnt[CTA_MODE_REAR][FBK_SIDE_RIGHT]   = 9u;
   cta_core_output.cta_brake_hold_cnt[CTA_MODE_FRONT][FBK_SIDE_LEFT]  = 2u;
   cta_core_output.cta_brake_hold_cnt[CTA_MODE_FRONT][FBK_SIDE_RIGHT] = 4u;
   cta_core_output.cta_brake_hold_cnt[CTA_MODE_REAR][FBK_SIDE_LEFT]   = 76u;
   cta_core_output.cta_brake_hold_cnt[CTA_MODE_REAR][FBK_SIDE_RIGHT]  = 34u;
   cta_core_output.cta_brake_supp_cnt[CTA_MODE_FRONT][FBK_SIDE_LEFT]  = 43u;
   cta_core_output.cta_brake_supp_cnt[CTA_MODE_FRONT][FBK_SIDE_RIGHT] = 12u;
   cta_core_output.cta_brake_supp_cnt[CTA_MODE_REAR][FBK_SIDE_LEFT]   = 76u;
   cta_core_output.cta_brake_supp_cnt[CTA_MODE_REAR][FBK_SIDE_RIGHT]  = 100u;

   cta_core_output.cta_status    = CTA_STATUS_ACTIVE;
   cta_core_output.f_cta_enabled = FBK_TRUE;

   expected_P0_x = -p_vehicle_data->host_length - cta_instance.calibration.k_cta_max_long_point_criticality_level[0][0];
   expected_P0_y = cta_instance.calibration.k_cta_butterfly_lat[0];
   expected_P1_x = -p_vehicle_data->host_length - cta_instance.calibration.k_cta_min_long_point_criticality_level[0][0];
   expected_P1_y = cta_instance.calibration.k_cta_butterfly_lat[1];
   expected_P2_x =
      expected_P1_x
      - (cta_instance.calibration.k_cta_max_length_fov * Fast_Tan(cta_instance.calibration.k_cta_angles_zone_definition[1]));
   expected_P2_y = cta_instance.calibration.k_cta_max_length_fov;
   expected_P3_x =
      expected_P0_x
      + (cta_instance.calibration.k_cta_max_length_fov * Fast_Tan(cta_instance.calibration.k_cta_angles_zone_definition[0]));
   expected_P3_y = cta_instance.calibration.k_cta_max_length_fov;

   /** \action Call Cta_Post_Run such that cta_output is filled accordingly. */
   Cta_Post_Run(&cta_instance, &cta_input, &cta_output);

   /** \assert Check that cta_output is filled correctly. */
   EXPECT_FLOAT_EQ(cta_output.cta_obj_ttc_left, 4.1f);
   EXPECT_FLOAT_EQ(cta_output.cta_obj_ttc_right, 7.0f);
   EXPECT_EQ(cta_output.cta_alert_level_left, 1u);
   EXPECT_EQ(cta_output.cta_alert_level_right, 2u);
   EXPECT_EQ(cta_output.cta_id_left, 14u);
   EXPECT_EQ(cta_output.cta_id_right, 21u);
   EXPECT_EQ(cta_output.cta_warn_hold_cnt_left, 1u);
   EXPECT_EQ(cta_output.cta_warn_hold_cnt_right, 9u);
   EXPECT_EQ(cta_output.cta_brake_hold_cnt_left, 76u);
   EXPECT_EQ(cta_output.cta_brake_hold_cnt_right, 34u);
   EXPECT_EQ(cta_output.cta_brake_supp_cnt_left, 76u);
   EXPECT_EQ(cta_output.cta_brake_supp_cnt_right, 100u);
   EXPECT_EQ(cta_output.f_brake_qualifier_left, FBK_FALSE);
   EXPECT_EQ(cta_output.f_brake_qualifier_right, FBK_TRUE);
   EXPECT_EQ(cta_output.f_cta_enabled, FBK_TRUE);
   EXPECT_EQ(cta_output.DBG_Crit_Zone_Right_P0_PositionX, expected_P0_x);
   EXPECT_EQ(cta_output.DBG_Crit_Zone_Right_P0_PositionY, expected_P0_y);
   EXPECT_EQ(cta_output.DBG_Crit_Zone_Right_P1_PositionX, expected_P1_x);
   EXPECT_EQ(cta_output.DBG_Crit_Zone_Right_P1_PositionY, expected_P1_y);
   EXPECT_EQ(cta_output.DBG_Crit_Zone_Right_P2_PositionX, expected_P2_x);
   EXPECT_EQ(cta_output.DBG_Crit_Zone_Right_P2_PositionY, expected_P2_y);
   EXPECT_EQ(cta_output.DBG_Crit_Zone_Right_P3_PositionX, expected_P3_x);
   EXPECT_EQ(cta_output.DBG_Crit_Zone_Right_P3_PositionY, expected_P3_y);
}

/**
 * Check if the STLA criticality zone is calculated correctly.
 * \uts{CSCSA-90087} \sdd{CSCSA-92601} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Get_Stla_Criticality_Zone__general_test)
{
   /** \arrange Setup set of objects in all zones */
   uint8_t idx;
   const uint8_t obj_number = 14u;
   struct Obj_Prop
   {
      uint8_t side;
      float32_T x;
      float32_T y;
      CTA_STLA_CRIT_ZONE_T expected_result;
   };
   Obj_Prop test_objects[obj_number];
   CTA_STLA_CRIT_ZONE_T results[obj_number];


   p_vehicle_data->host_width  = 2.0f;
   p_vehicle_data->host_length = 4.0f;

   /* helpers with zone borders*/
   float32_T line_g  = -Fbk_Half(data.vehicle_data.host_width) - p_cta_custom_cals->k_stla_crit_zone_G_E_line;
   float32_T line_e  = -Fbk_Half(data.vehicle_data.host_width);
   float32_T line_j  = Fbk_Half(data.vehicle_data.host_width);
   float32_T line_l  = Fbk_Half(data.vehicle_data.host_width) + p_cta_custom_cals->k_stla_crit_zone_G_E_line;
   float32_T line_d  = FBK_ZERO_F;
   float32_T line_c  = line_d - p_cta_custom_cals->k_stla_crit_zone_D_C_line;
   float32_T line_n  = -data.vehicle_data.host_length;
   float32_T line_q  = line_n - p_cta_custom_cals->k_stla_crit_zone_N_Q_line;
   float32_T line_qh = line_q - p_cta_custom_cals->k_stla_crit_zone_Q_QH_line;

   /* left objects*/
   test_objects[0] = {FBK_SIDE_LEFT, Fbk_Half(line_d + line_c), (line_g - FBK_ONE_F), CTA_STLA_CRIT_ZONE_NONE};
   test_objects[1] = {FBK_SIDE_LEFT, (line_d + FBK_ONE_F), Fbk_Half(line_g + line_e), CTA_STLA_CRIT_ZONE_NONE};
   test_objects[2] = {FBK_SIDE_LEFT, Fbk_Half(line_d + line_c), Fbk_Half(line_g + line_e), CTA_STLA_CRIT_ZONE_1};
   test_objects[3] = {FBK_SIDE_LEFT, Fbk_Half(line_c + line_n), Fbk_Half(line_g + line_e), CTA_STLA_CRIT_ZONE_3};
   test_objects[4] = {FBK_SIDE_LEFT, Fbk_Half(line_n + line_q), Fbk_Half(line_g + line_e), CTA_STLA_CRIT_ZONE_5};
   test_objects[5] = {FBK_SIDE_LEFT, Fbk_Half(line_q + line_qh), Fbk_Half(line_g + line_e), CTA_STLA_CRIT_ZONE_7};
   test_objects[6] = {FBK_SIDE_LEFT, (line_qh - FBK_ONE_F), Fbk_Half(line_g + line_e), CTA_STLA_CRIT_ZONE_NONE};

   /* right objects */
   test_objects[7]  = {FBK_SIDE_RIGHT, Fbk_Half(line_d + line_c), (line_l + FBK_ONE_F), CTA_STLA_CRIT_ZONE_NONE};
   test_objects[8]  = {FBK_SIDE_RIGHT, (line_d + FBK_ONE_F), Fbk_Half(line_l + line_j), CTA_STLA_CRIT_ZONE_NONE};
   test_objects[9]  = {FBK_SIDE_RIGHT, Fbk_Half(line_d + line_c), Fbk_Half(line_l + line_j), CTA_STLA_CRIT_ZONE_2};
   test_objects[10] = {FBK_SIDE_RIGHT, Fbk_Half(line_c + line_n), Fbk_Half(line_l + line_j), CTA_STLA_CRIT_ZONE_4};
   test_objects[11] = {FBK_SIDE_RIGHT, Fbk_Half(line_n + line_q), Fbk_Half(line_l + line_j), CTA_STLA_CRIT_ZONE_6};
   test_objects[12] = {FBK_SIDE_RIGHT, Fbk_Half(line_q + line_qh), Fbk_Half(line_l + line_j), CTA_STLA_CRIT_ZONE_8};
   test_objects[13] = {FBK_SIDE_RIGHT, (line_qh - FBK_ONE_F), Fbk_Half(line_l + line_j), CTA_STLA_CRIT_ZONE_NONE};

   /* set tracker data*/
   for (idx = 0; idx < obj_number; idx++)
   {
      object_data[idx].vcs_pos.x = test_objects[idx].x;
      object_data[idx].vcs_pos.y = test_objects[idx].y;
   }

   /** \action Call function on set of objects. */
   for (idx = 0; idx < obj_number; idx++)
   {
      results[idx] = Cta_Get_Stla_Criticality_Zone(idx, &data, p_cta_custom_cals, test_objects[idx].side);
   }

   /** \assert Check that zones are returned correctly */
   for (idx = 0; idx < obj_number; idx++)
   {
      EXPECT_EQ(results[idx], test_objects[idx].expected_result);
   }
}