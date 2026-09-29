/**
 * @file ced_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Honda_SRR6 CTA post run
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42102}
 */

#include "cta_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "cta_post_run.c"
#include "fbk_macros.h"
}

/**
 * Check that post run initialization routine resets the honda_srr6 output accordingly.
 * \uts{CSCSA-42103} \sdd{SF-3954} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Post_Run_Init__check_initialization_routine)
{
   /** \arrange set output to non default */
   Cta_Honda_Persistent.cta_prev_id_left   = 1u;
   Cta_Honda_Persistent.cta_prev_id_right  = 2u;
   Cta_Honda_Persistent.cta_prev_ttc_left  = 3.0f;
   Cta_Honda_Persistent.cta_prev_ttc_right = 2.0f;
   /** \action Run function to test */
   Cta_Post_Run_Init(&cta_instance);
   /** \assert Verify output is set to default */
   EXPECT_EQ(Cta_Honda_Persistent.cta_prev_id_left, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(Cta_Honda_Persistent.cta_prev_ttc_left, CTA_HIGH_DEFAULT_VAL);
   EXPECT_EQ(Cta_Honda_Persistent.cta_prev_id_right, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(Cta_Honda_Persistent.cta_prev_ttc_right, CTA_HIGH_DEFAULT_VAL);
}

/**
 * Check that post run initialization routine resets the honda_srr6 output accordingly.
 * \uts{CSCSA-42104} \sdd{SF-4014} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Reset_Output__check_reset_routine)
{
   /** \arrange set output to non default */
   cta_output.f_cta_enabled                  = 1u;
   cta_output.f_cta_warn_left                = 1u;
   cta_output.f_cta_alert_left               = 1u;
   cta_output.cta_id_left                    = 1u;
   cta_output.cta_ttc_left                   = 2.9f;
   cta_output.cta_objPoseX_left              = 2.9f;
   cta_output.cta_objPoseY_left              = 2.9f;
   cta_output.cta_objVelocityX_left          = 2.9f;
   cta_output.cta_objVelocityY_left          = 2.9f;
   cta_output.cta_heading_left               = 1.0f;
   cta_output.cta_intersection_point_x_left  = 2.0f;
   cta_output.f_cta_warn_right               = 1u;
   cta_output.f_cta_alert_right              = 1u;
   cta_output.cta_id_right                   = 1u;
   cta_output.cta_ttc_right                  = 2.0f;
   cta_output.cta_objPoseX_right             = 2.0f;
   cta_output.cta_objPoseY_right             = 2.0f;
   cta_output.cta_objVelocityX_right         = 2.0f;
   cta_output.cta_objVelocityY_right         = 2.0f;
   cta_output.cta_heading_right              = 1u;
   cta_output.cta_intersection_point_x_right = 2.0f;
   cta_output.f_brake_qualifier              = 1u;

   /** \action Run function to test */
   Cta_Reset_Output(&cta_output);

   /** \assert Verify output is set to default */

   EXPECT_EQ(cta_output.f_cta_enabled, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_warn_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_alert_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_id_left, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_left, CTA_HIGH_DEFAULT_VAL);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseX_left, CTA_HIGH_DEFAULT_VAL);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseY_left, CTA_HIGH_DEFAULT_VAL);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityX_left, CTA_HIGH_DEFAULT_VAL);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityY_left, CTA_HIGH_DEFAULT_VAL);
   EXPECT_FLOAT_EQ(cta_output.cta_heading_left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_intersection_point_x_left, CTA_HIGH_DEFAULT_VAL);
   EXPECT_EQ(cta_output.f_cta_warn_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_alert_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_id_right, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_right, CTA_HIGH_DEFAULT_VAL);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseX_right, CTA_HIGH_DEFAULT_VAL);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseY_right, CTA_HIGH_DEFAULT_VAL);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityX_right, CTA_HIGH_DEFAULT_VAL);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityY_right, CTA_HIGH_DEFAULT_VAL);
   EXPECT_FLOAT_EQ(cta_output.cta_heading_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_intersection_point_x_right, CTA_HIGH_DEFAULT_VAL);
   EXPECT_EQ(cta_output.f_brake_qualifier, FBK_ZERO_UINT);
}

/**
 * Check that Cta_Set_Output sets cta customer output properly
 * \uts{CSCSA-42105} \sdd{SF-4015} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output__populate_customer_output_as_core_output_left_side_critical_lvl_alert)
{
   /** \arrange set output to non default */
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT] = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT]          = 1u;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT]     = 2.0f;

   /** \action Run function to test */
   Cta_Set_Output(&cta_output, &cta_instance);

   /** \assert Check all outputs are as expected */
   EXPECT_EQ(cta_output.cta_id_left, 1u);
   EXPECT_EQ(cta_output.f_cta_alert_left, FBK_ONE_UINT);
   EXPECT_EQ(cta_output.f_cta_warn_left, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_left, 2.0f);
   EXPECT_EQ(cta_output.f_cta_alert_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_id_left, Cta_Honda_Persistent.cta_prev_id_left);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_left, Cta_Honda_Persistent.cta_prev_ttc_left);
}


/**
 * Check that Cta_Set_Output sets cta customer output properly
 * \uts{} \sdd{SF-4015} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output__suppress_critical_lvl_alert_left_dist_above_thresh_right_corner)
{
   /** \arrange set output to non default */
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT] = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT]          = 1u;
   cta_instance.core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT]       = 0u;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT]     = 2.0f;
   data.object_data[0u].length                                            = 4.0f;
   data.object_data[0u].width                                             = 2.0f;
   data.object_data[0u].vcs_pos.y                                         = -30.0f;
   data.object_data[0u].vcs_heading                                       = 1.5;

   /** \action Run function to test */
   Cta_Set_Output(&cta_output, &cta_instance);

   /** \assert Check all outputs are as expected */
   EXPECT_EQ(cta_output.cta_id_left, 1u);
   EXPECT_EQ(cta_output.f_cta_alert_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_warn_left, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_left, 2.0f);
   EXPECT_EQ(cta_output.f_cta_alert_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_id_left, Cta_Honda_Persistent.cta_prev_id_left);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_left, Cta_Honda_Persistent.cta_prev_ttc_left);
}


/**
 * Check that Cta_Set_Output sets cta customer output properly
 * \uts{} \sdd{SF-4015} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output__suppress_critical_lvl_alert_left_dist_above_thresh_left_corner)
{
   /** \arrange set output to non default */
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT] = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT]          = 1u;
   cta_instance.core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT]       = 0u;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT]     = 2.0f;
   data.object_data[0u].length                                            = 4.0f;
   data.object_data[0u].width                                             = 2.0f;
   data.object_data[0u].vcs_pos.y                                         = -30.0f;
   data.object_data[0u].vcs_heading                                       = -4.5;

   /** \action Run function to test */
   Cta_Set_Output(&cta_output, &cta_instance);

   /** \assert Check all outputs are as expected */
   EXPECT_EQ(cta_output.cta_id_left, 1u);
   EXPECT_EQ(cta_output.f_cta_alert_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_warn_left, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_left, 2.0f);
   EXPECT_EQ(cta_output.f_cta_alert_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_id_left, Cta_Honda_Persistent.cta_prev_id_left);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_left, Cta_Honda_Persistent.cta_prev_ttc_left);
}


/**
 * Check that Cta_Set_Output sets cta customer output properly
 * \uts{} \sdd{SF-4015} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output__suppress_critical_lvl_alert_right_dist_above_thresh_right_corner)
{
   /** \arrange set output to non default */
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT] = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]          = 1u;
   cta_instance.core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT]        = 0u;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = 2.0f;
   data.object_data[0u].length                                             = 4.0f;
   data.object_data[0u].width                                              = 2.0f;
   data.object_data[0u].vcs_pos.y                                          = 30.0f;
   data.object_data[0u].vcs_heading                                        = -1.5f;

   /** \action Run function to test */
   Cta_Set_Output(&cta_output, &cta_instance);

   /** \assert Check all outputs are as expected */
   EXPECT_EQ(cta_output.cta_id_right, 1u);
   EXPECT_EQ(cta_output.f_cta_alert_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_warn_right, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_right, 2.0f);
   EXPECT_EQ(cta_output.f_cta_alert_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_id_right, Cta_Honda_Persistent.cta_prev_id_right);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_right, Cta_Honda_Persistent.cta_prev_ttc_right);
}

/**
 * Check that Cta_Set_Output sets cta customer output properly
 * \uts{} \sdd{SF-4015} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output__suppress_critical_lvl_alert_right_dist_above_thresh_left_corner)
{
   /** \arrange set output to non default */
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT] = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]          = 1u;
   cta_instance.core_output.cta_index[CTA_MODE_REAR][FBK_SIDE_LEFT]        = 0u;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = 2.0f;
   data.object_data[0u].length                                             = 4.0f;
   data.object_data[0u].width                                              = 2.0f;
   data.object_data[0u].vcs_pos.y                                          = 30.0f;
   data.object_data[0u].vcs_heading                                        = 4.5f;

   /** \action Run function to test */
   Cta_Set_Output(&cta_output, &cta_instance);

   /** \assert Check all outputs are as expected */
   EXPECT_EQ(cta_output.cta_id_right, 1u);
   EXPECT_EQ(cta_output.f_cta_alert_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_warn_right, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_right, 2.0f);
   EXPECT_EQ(cta_output.f_cta_alert_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_id_right, Cta_Honda_Persistent.cta_prev_id_right);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_right, Cta_Honda_Persistent.cta_prev_ttc_right);
}


/**
 * Check that Cta_Set_Output sets cta customer output properly
 * \uts{CSCSA-42106} \sdd{SF-4015} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output__populate_customer_output_as_core_output_left_side_critical_level_warning)
{
   /** \arrange set output to non default */
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT] = CTA_CRIT_LEVEL_1;
   cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT]          = 1u;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT]     = 2.0f;

   /** \action Run function to test */
   Cta_Set_Output(&cta_output, &cta_instance);

   /** \assert Check all outputs are as expected */
   EXPECT_EQ(cta_output.cta_id_left, 1u);
   EXPECT_EQ(cta_output.f_cta_alert_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_warn_left, FBK_ONE_UINT);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_left, 2.0f);
   EXPECT_EQ(cta_output.f_cta_alert_right, FBK_ZERO_UINT);
}

/**
 * Check that Cta_Set_Output sets cta customer output properly
 * \uts{CSCSA-42107} \sdd{SF-4015} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output__populate_customer_output_as_core_output_right_side_critical_level_alert)
{
   /** \arrange set output to non default */
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT] = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]          = 3u;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = 2.0f;

   /** \action Run function to test */
   Cta_Set_Output(&cta_output, &cta_instance);

   /** \assert Check all outputs are as expected */
   EXPECT_EQ(cta_output.cta_id_right, 3u);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_right, 2.0f);
   EXPECT_EQ(cta_output.f_cta_alert_right, FBK_ONE_UINT);
   EXPECT_EQ(cta_output.f_cta_warn_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_alert_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_id_right, Cta_Honda_Persistent.cta_prev_id_right);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_right, Cta_Honda_Persistent.cta_prev_ttc_right);
}

/**
 * Check that Cta_Set_Output sets cta customer output properly
 * \uts{CSCSA-42108} \sdd{SF-4015} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output__populate_customer_output_as_core_output_right_side_critical_level_warning)
{
   /** \arrange set output to non default */
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT] = CTA_CRIT_LEVEL_1;
   cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]          = 3u;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = 2.0f;

   /** \action Run function to test */
   Cta_Set_Output(&cta_output, &cta_instance);

   /** \assert Check all outputs are as expected */
   EXPECT_EQ(cta_output.cta_id_right, 3u);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_right, 2.0f);
   EXPECT_EQ(cta_output.f_cta_alert_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_warn_right, FBK_ONE_UINT);
   EXPECT_EQ(cta_output.f_cta_alert_left, FBK_ZERO_UINT);
}

/**
 * Check that Cta_Set_Output sets cta customer output properly
 * \uts{CSCSA-42109} \sdd{SF-4015} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output__check_brake_qualifier)
{
   /** \arrange set output to non default */
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT]   = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]            = 3u;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT]       = 2.0f;
   cta_instance.core_output.f_brake_qualifier[CTA_MODE_REAR][FBK_SIDE_RIGHT] = FBK_TRUE;

   /** \action Run function to test */
   Cta_Set_Output(&cta_output, &cta_instance);

   /** \assert Check all outputs are as expected */
   EXPECT_EQ(cta_output.cta_id_right, 3u);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_right, 2.0f);
   EXPECT_EQ(cta_output.f_cta_alert_right, FBK_ONE_UINT);
   EXPECT_EQ(cta_output.f_cta_warn_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_brake_qualifier, FBK_ONE_UINT);
}

/**
 * Check that Cta_Set_Output sets cta customer output properly when alert is holding
 * \uts{CSCSA-60542} \sdd{SF-4015} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output__populate_customer_output_core_is_holding_alert)
{
   /** \arrange set output to non default, core ID = 0, but alert ON */
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT]  = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT]           = FBK_ZERO_UINT;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT]      = 0.0f;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT] = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]          = FBK_ZERO_UINT;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT]     = 0.0f;

   Cta_Honda_Persistent.cta_prev_id_left   = 1u;
   Cta_Honda_Persistent.cta_prev_ttc_left  = 1.0f;
   Cta_Honda_Persistent.cta_prev_id_right  = 2u;
   Cta_Honda_Persistent.cta_prev_ttc_right = 2.0f;

   /** \action Run function to test */
   Cta_Set_Output(&cta_output, &cta_instance);

   /** \assert Check that previus data is passed to current putput */
   EXPECT_EQ(cta_output.cta_id_left, 1u);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_left, 1.0f);
   EXPECT_EQ(cta_output.cta_id_right, 2u);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_right, 2.0f);
}