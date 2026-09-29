/**
 * @file lcda_pre_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for lcda_pre_run.c functions
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{}
 */

#include "lcda_pre_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_iface.h"
#include "fbk_macros.h"
#include "lcda_pre_run.c"
#include "lcda_types.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}


/**
 * Check that mapping in Lcda_Pre_Run is correct.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Pre_Run__check_correct_mapping)
{
   /** \arrange Set up FBK, Vehicle Data, LCDA input and calibration values, which are used in Lcda_Pre_Run with arbitrary values.
    */
   p_vehicle_data->lane_width         = 3.5f;
   p_vehicle_data->lane_center_offset = 0.5f;
   Fbk_Update_Index_Id_Lookup_Table(&lookup_table, &data);

   Lcda_Init_Input(&lcda_input);
   Lcda_Pre_Run_Init(&lcda_instance);
   lcda_input.f_lcda_enable     = FBK_TRUE;
   lcda_input.f_lcda_enable_bsw = FBK_TRUE;
   lcda_input.f_lcda_enable_cvw = FBK_TRUE;

   cals.k_cvw_ttc                        = 2.5f;
   cals.k_lcda_f_enable_fallback_handler = FBK_FALSE;


   /** \action Call Lcda_Pre_Run, such that lcda_core_input is filled accordingly. */
   Lcda_Pre_Run(&lcda_instance, &lcda_input, &fbk_output);

   /** \assert Check that lcda_core_input is filled correctly. */
   EXPECT_TRUE(lcda_core_input.enabled_flags.f_lcda_enabled);
   EXPECT_TRUE(lcda_core_input.enabled_flags.f_bsw_enabled);
   EXPECT_TRUE(lcda_core_input.enabled_flags.f_cvw_enabled);
   EXPECT_FALSE(lcda_core_input.enabled_flags.f_slc_enabled);
   EXPECT_FALSE(lcda_core_input.enabled_flags.f_elc_enabled);
   EXPECT_FALSE(lcda_core_input.enabled_flags.f_dropback_enabled);
   EXPECT_FALSE(lcda_core_input.enabled_flags.f_fallback_enabled);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.cvw_ttc_threshold, 2.5f);
   EXPECT_FALSE(lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone);
   EXPECT_EQ(lcda_core_input.bsw_zone_calculation_mode, BSW_ZONE_CALC_FIXED_INPUT);
   EXPECT_FLOAT_EQ(lcda_core_input.lane_width, 3.5f);
   EXPECT_FLOAT_EQ(lcda_core_input.lane_center_offset, 0.5f);
}

/**
 * Check the CVW data with respect to trailer settings.
 * Existing trailer shall disable CVW functionality.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Pre_Run__check_correct_mapping_trailer_attached)
{
   /** \arrange Set up LCDA input and calibration values, which are used in Lcda_Pre_Run with arbitrary values. */
   p_vehicle_data->lane_width         = 3.5f;
   p_vehicle_data->lane_center_offset = 0.5f;
   Fbk_Update_Index_Id_Lookup_Table(&lookup_table, &data);

   Lcda_Pre_Run_Init(&lcda_instance);

   lcda_input.f_lcda_enable     = FBK_TRUE;
   lcda_input.f_lcda_enable_bsw = FBK_TRUE;
   lcda_input.f_lcda_enable_cvw = FBK_TRUE;
   lcda_input.f_trailer_present = FBK_TRUE;
   lcda_input.trailer_length    = 1.0f;
   lcda_input.trailer_width     = 0.5f;
   lcda_input.trailer_angle     = 0.1f;

   cals.k_cvw_ttc                        = 2.5f;
   cals.k_lcda_f_enable_fallback_handler = FBK_TRUE;

   /** \action Call Lcda_Pre_Run, such that lcda_core_input is filled accordingly. */
   Lcda_Pre_Run(&lcda_instance, &lcda_input, &fbk_output);

   /** \assert Check that lcda_core_input is filled correctly. */
   EXPECT_TRUE(lcda_core_input.enabled_flags.f_lcda_enabled);
   EXPECT_TRUE(lcda_core_input.enabled_flags.f_bsw_enabled);
   EXPECT_FALSE(lcda_core_input.enabled_flags.f_cvw_enabled);
   EXPECT_FALSE(lcda_core_input.enabled_flags.f_slc_enabled);
   EXPECT_FALSE(lcda_core_input.enabled_flags.f_elc_enabled);
   EXPECT_FALSE(lcda_core_input.enabled_flags.f_dropback_enabled);
   EXPECT_TRUE(lcda_core_input.enabled_flags.f_fallback_enabled);

   EXPECT_TRUE(lcda_core_input.trailer.f_trailer_present);
   EXPECT_FLOAT_EQ(lcda_core_input.trailer.length, 1.0f);
   EXPECT_FLOAT_EQ(lcda_core_input.trailer.width, 0.5f);
   EXPECT_FLOAT_EQ(lcda_core_input.trailer.angle, 0.1f);

   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.cvw_ttc_threshold, 2.5f);
   EXPECT_FALSE(lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone);
   EXPECT_EQ(lcda_core_input.bsw_zone_calculation_mode, BSW_ZONE_CALC_FIXED_INPUT);
   EXPECT_FLOAT_EQ(lcda_core_input.lane_width, 3.5f);
   EXPECT_FLOAT_EQ(lcda_core_input.lane_center_offset, 0.5f);
}

/**
 * Check that BSW and CVW zone is set correctly from basic calibration parameters.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Lcda_Pre_Run_Test, Lcda_Thunder_Create_Initial_Zones__general_test)
{
   /** \arrange Set up LCDA calibration for zone dimensions. */
   float32_T ego_front_x     = 0.0f;
   float32_T out_rear_x      = -10.0f;
   float32_T ego_front_y     = 1.0f;
   float32_T out_rear_y      = 4.0f;
   float32_T ego_front_x_hys = 0.5f;
   float32_T out_rear_x_hys  = 1.0f;
   float32_T ego_front_y_hys = 0.5f;
   float32_T out_rear_y_hys  = 1.0f;
   float32_T ego_width       = 2.0f;
   float32_T ego_length      = 5.0f;

   cals.k_bsw_zone_front_ego_side_x  = ego_front_x;
   cals.k_bsw_zone_rear_outer_side_x = out_rear_x;
   cals.k_bsw_zone_front_ego_side_y  = ego_front_y;
   cals.k_bsw_zone_rear_outer_side_y = out_rear_y;

   cals.k_bsw_zone_front_ego_side_x_hys  = ego_front_x_hys;
   cals.k_bsw_zone_rear_outer_side_x_hys = out_rear_x_hys;
   cals.k_bsw_zone_front_ego_side_y_hys  = ego_front_y_hys;
   cals.k_bsw_zone_rear_outer_side_y_hys = out_rear_y_hys;

   p_vehicle_data->host_length = ego_length;
   p_vehicle_data->host_width  = ego_width;

   lcda_input.f_trailer_present = FBK_FALSE;

   /** \action Call function setting zones. */
   Lcda_Thunder_Create_Initial_Zones(&lcda_core_input, &cals);

   /** \assert Check that zones for both BSW and CVW, including hysteresis are calculated corretly */
   EXPECT_EQ(lcda_core_input.bsw_zone_calculation_mode, BSW_ZONE_CALC_FIXED_INPUT);
   EXPECT_EQ(lcda_core_input.initial_bsw_zone.size, LCDA_NUMBER_OF_ZONE_POINTS);
   EXPECT_EQ(lcda_core_input.initial_bsw_zone_hys.size, LCDA_NUMBER_OF_ZONE_POINTS);

   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone.points[FRONT_EGO_SIDE].x, ego_front_x);
   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone.points[FRONT_EGO_SIDE].y, ego_front_y);

   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone.points[FRONT_OUTER_SIDE].x, ego_front_x);
   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone.points[FRONT_OUTER_SIDE].y, out_rear_y);

   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone.points[MIDDLE_OUTER_SIDE].x, Fbk_Half(ego_front_x + out_rear_x));
   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone.points[MIDDLE_OUTER_SIDE].y, out_rear_y);

   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone.points[REAR_OUTER_SIDE].x, out_rear_x);
   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone.points[REAR_OUTER_SIDE].y, out_rear_y);

   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone.points[REAR_EGO_SIDE].x, out_rear_x);
   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone.points[REAR_EGO_SIDE].y, ego_front_y);

   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone.points[MIDDLE_EGO_SIDE].x, Fbk_Half(ego_front_x + out_rear_x));
   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone.points[MIDDLE_EGO_SIDE].y, ego_front_y);

   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone_hys.points[FRONT_EGO_SIDE].x, ego_front_x + ego_front_x_hys);
   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone_hys.points[FRONT_EGO_SIDE].y, ego_front_y - ego_front_y_hys);

   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone_hys.points[FRONT_OUTER_SIDE].x, ego_front_x + ego_front_x_hys);
   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone_hys.points[FRONT_OUTER_SIDE].y, out_rear_y + out_rear_y_hys);

   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone_hys.points[MIDDLE_OUTER_SIDE].x, Fbk_Half(ego_front_x + out_rear_x));
   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone_hys.points[MIDDLE_OUTER_SIDE].y, out_rear_y + out_rear_y_hys);

   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone_hys.points[REAR_OUTER_SIDE].x, out_rear_x - out_rear_x_hys);
   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone_hys.points[REAR_OUTER_SIDE].y, out_rear_y + out_rear_y_hys);

   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone_hys.points[REAR_EGO_SIDE].x, out_rear_x - out_rear_x_hys);
   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone_hys.points[REAR_EGO_SIDE].y, ego_front_y - ego_front_y_hys);

   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone_hys.points[MIDDLE_EGO_SIDE].x, Fbk_Half(ego_front_x + out_rear_x));
   EXPECT_FLOAT_EQ(lcda_core_input.initial_bsw_zone_hys.points[MIDDLE_EGO_SIDE].y, ego_front_y - ego_front_y_hys);

   EXPECT_EQ(lcda_core_input.initial_cvw_zone.size, LCDA_NUMBER_OF_ZONE_POINTS);
   EXPECT_EQ(lcda_core_input.initial_cvw_zone_hys.size, LCDA_NUMBER_OF_ZONE_POINTS);

   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone.points[FRONT_OUTER_SIDE].x, ego_front_x);
   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone.points[FRONT_OUTER_SIDE].y, out_rear_y);

   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone.points[MIDDLE_OUTER_SIDE].x, Fbk_Half(ego_front_x - cals.k_lcda_max_range));
   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone.points[MIDDLE_OUTER_SIDE].y, out_rear_y);

   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone.points[REAR_OUTER_SIDE].x, -cals.k_lcda_max_range);
   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone.points[REAR_OUTER_SIDE].y, out_rear_y);

   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone.points[REAR_EGO_SIDE].x, -cals.k_lcda_max_range);
   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone.points[REAR_EGO_SIDE].y, Fbk_Half(ego_width));

   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone.points[MIDDLE_EGO_SIDE].x, Fbk_Half(ego_front_x - cals.k_lcda_max_range));
   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone.points[MIDDLE_EGO_SIDE].y, Fbk_Half(ego_width));

   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone.points[FRONT_EGO_SIDE].x, ego_front_x);
   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone.points[FRONT_EGO_SIDE].y, Fbk_Half(ego_width));

   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone_hys.points[FRONT_OUTER_SIDE].x, ego_front_x);
   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone_hys.points[FRONT_OUTER_SIDE].y, out_rear_y + out_rear_y_hys);

   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone_hys.points[MIDDLE_OUTER_SIDE].x, Fbk_Half(ego_front_x - cals.k_lcda_max_range));
   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone_hys.points[MIDDLE_OUTER_SIDE].y, out_rear_y + out_rear_y_hys);

   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone_hys.points[REAR_OUTER_SIDE].x, -cals.k_lcda_max_range);
   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone_hys.points[REAR_OUTER_SIDE].y, out_rear_y + out_rear_y_hys);

   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone_hys.points[REAR_EGO_SIDE].x, -cals.k_lcda_max_range);
   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone_hys.points[REAR_EGO_SIDE].y, ego_front_y - ego_front_y_hys);

   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone_hys.points[MIDDLE_EGO_SIDE].x, Fbk_Half(ego_front_x - cals.k_lcda_max_range));
   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone_hys.points[MIDDLE_EGO_SIDE].y, ego_front_y - ego_front_y_hys);

   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone_hys.points[FRONT_EGO_SIDE].x, ego_front_x);
   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone_hys.points[FRONT_EGO_SIDE].y, ego_front_y - ego_front_y_hys);
}
