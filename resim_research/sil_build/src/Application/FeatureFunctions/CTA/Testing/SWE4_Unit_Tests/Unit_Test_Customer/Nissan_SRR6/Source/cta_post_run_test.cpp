/**
 * @file cta_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for nissan srr6 cta post run
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42125}
 */

#include "cta_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "cta_post_run.c"
#include "cta_types.h"
#include "fbk_macros.h"
#include "fbk_ref_point.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
}

/**
 * Check that initialization routine maps correctly to default.
 * \uts{CSCSA-42126} \sdd{SF-3990} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Reset_Output__default_for_output)
{
   /** \arrange set cta output to something different than default. */
   cta_output.f_cta_enabled = FBK_ONE_UINT;

   cta_output.f_cta_alert_left              = FBK_ONE_UINT;
   cta_output.cta_id_left                   = FBK_ONE_UINT;
   cta_output.cta_ttc_left                  = FBK_ONE_F;
   cta_output.cta_objPoseX_left             = FBK_ONE_F;
   cta_output.cta_objPoseY_left             = FBK_ONE_F;
   cta_output.cta_objVelocityX_left         = FBK_ONE_F;
   cta_output.cta_objVelocityY_left         = FBK_ONE_F;
   cta_output.cta_heading_left              = FBK_ONE_F;
   cta_output.cta_intersection_point_x_left = FBK_ONE_F;

   cta_output.f_cta_alert_right              = FBK_ONE_UINT;
   cta_output.cta_id_right                   = FBK_ONE_UINT;
   cta_output.cta_ttc_right                  = FBK_ONE_F;
   cta_output.cta_objPoseX_right             = FBK_ONE_F;
   cta_output.cta_objPoseY_right             = FBK_ONE_F;
   cta_output.cta_objVelocityX_right         = FBK_ONE_F;
   cta_output.cta_objVelocityY_right         = FBK_ONE_F;
   cta_output.cta_heading_right              = FBK_ONE_F;
   cta_output.cta_intersection_point_x_right = FBK_ONE_F;

   /** \action execute reset of sfe output */
   Cta_Reset_Output(&cta_output);

   /** \assert expect output to be set to default */
   EXPECT_EQ(cta_output.f_cta_enabled, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.f_cta_alert_left, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_id_left, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_left, CTA_HIGH_DEFAULT_VAL);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseX_left, CTA_DEFAULT_NISSAN_SRR6_POS_LONG);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseY_left, CTA_DEFAULT_NISSAN_SRR6_POS_LAT);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityX_left, CTA_DEFAULT_NISSAN_SRR6_VEL_LONG);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityY_left, CTA_DEFAULT_NISSAN_SRR6_VEL_LAT);
   EXPECT_FLOAT_EQ(cta_output.cta_heading_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_intersection_point_x_right, FBK_ZERO_F);
   EXPECT_EQ(cta_output.f_cta_alert_right, FBK_ZERO_UINT);
   EXPECT_EQ(cta_output.cta_id_right, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_right, CTA_HIGH_DEFAULT_VAL);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseX_right, CTA_DEFAULT_NISSAN_SRR6_POS_LONG);
   EXPECT_FLOAT_EQ(cta_output.cta_objPoseY_right, CTA_DEFAULT_NISSAN_SRR6_POS_LAT);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityX_right, CTA_DEFAULT_NISSAN_SRR6_VEL_LONG);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityY_right, CTA_DEFAULT_NISSAN_SRR6_VEL_LAT);
   EXPECT_FLOAT_EQ(cta_output.cta_heading_right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(cta_output.cta_intersection_point_x_right, FBK_ZERO_F);
}


/**
 * Check CTA output setting function. Here alerts are set on both side by core
 * \uts{CSCSA-42128} \sdd{SF-3988} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output__alert_on_both_sides)
{
   /** \arrange set cta core output and tracker signals consumed at customer output interface. */
   cta_instance.core_output.f_cta_enabled                                        = FBK_TRUE;
   cta_instance.core_output.cta_status                                           = CTA_STATUS_ACTIVE;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT]        = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT]                 = FBK_ZERO_UINT;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT]            = FBK_ONE_F;
   cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_LEFT]            = FBK_ONE_F;
   cta_instance.core_output.cta_long_intersection[CTA_MODE_REAR][FBK_SIDE_LEFT]  = FBK_ONE_F;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT]       = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]                = FBK_ZERO_UINT;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT]           = FBK_ONE_F;
   cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_RIGHT]           = FBK_ONE_F;
   cta_instance.core_output.cta_long_intersection[CTA_MODE_REAR][FBK_SIDE_RIGHT] = FBK_ONE_F;

   object_data[cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT]].vcs_vel.x = 10.0f;
   object_data[cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT]].vcs_vel.y = 1.0f;

   object_data[cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]].vcs_vel.x = 10.0f;
   object_data[cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]].vcs_vel.y = 1.0f;

   /** \action execute output setter for cta */
   Cta_Set_Output(&cta_output, &cta_instance, p_vehicle_data);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(cta_output.f_cta_enabled, FBK_ONE_UINT);

   EXPECT_EQ(cta_output.f_cta_alert_left, FBK_TRUE);
   EXPECT_FLOAT_EQ(cta_output.cta_heading_left, cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   EXPECT_EQ(cta_output.cta_id_left, cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   EXPECT_FLOAT_EQ(cta_output.cta_intersection_point_x_left,
                   cta_instance.core_output.cta_long_intersection[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityX_left,
                   object_data[cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT]].vcs_vel.x);
   // Sign changes because output is in customer coordinate system
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityY_left,
                   -object_data[cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT]].vcs_vel.y);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_left, cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT]);

   EXPECT_EQ(cta_output.f_cta_alert_right, FBK_TRUE);
   EXPECT_FLOAT_EQ(cta_output.cta_heading_right, cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
   EXPECT_EQ(cta_output.cta_id_right, cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
   EXPECT_FLOAT_EQ(cta_output.cta_intersection_point_x_right,
                   cta_instance.core_output.cta_long_intersection[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityX_right,
                   object_data[cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]].vcs_vel.x);
   // Sign changes because output is in customer coordinate system
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityY_right,
                   -object_data[cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]].vcs_vel.y);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_right, cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
}


/**
 * Check CTA output setting function. Here CTA is disabled is set in the core output
 * \uts{CSCSA-42129} \sdd{SF-3988} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Set_Output__cta_disabled)
{
   /** \arrange set cta core output and tracker signals consumed at customer output interface. */
   cta_instance.core_output.cta_status                                           = CTA_STATUS_DISABLED;
   cta_instance.core_output.f_cta_enabled                                        = FBK_FALSE;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_LEFT]        = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT]                 = FBK_ZERO_UINT;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT]            = FBK_ONE_F;
   cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_LEFT]            = FBK_ONE_F;
   cta_instance.core_output.cta_long_intersection[CTA_MODE_REAR][FBK_SIDE_LEFT]  = FBK_ONE_F;
   cta_instance.core_output.cta_alert_level[CTA_MODE_REAR][FBK_SIDE_RIGHT]       = CTA_CRIT_LEVEL_2;
   cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]                = FBK_ZERO_UINT;
   cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT]           = FBK_ONE_F;
   cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_RIGHT]           = FBK_ONE_F;
   cta_instance.core_output.cta_long_intersection[CTA_MODE_REAR][FBK_SIDE_RIGHT] = FBK_ONE_F;

   object_data[cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT]].vcs_vel.x = 10.0f;
   object_data[cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT]].vcs_vel.y = 1.0f;

   object_data[cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]].vcs_vel.x = 10.0f;
   object_data[cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]].vcs_vel.y = 1.0f;

   /** \action execute output setter for cta */
   Cta_Set_Output(&cta_output, &cta_instance, p_vehicle_data);

   /** \assert expect that mapping is correctly set */
   EXPECT_EQ(cta_output.f_cta_enabled, FBK_ZERO_UINT);

   EXPECT_EQ(cta_output.f_cta_alert_left, FBK_TRUE);
   EXPECT_FLOAT_EQ(cta_output.cta_heading_left, cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   EXPECT_EQ(cta_output.cta_id_left, cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   EXPECT_FLOAT_EQ(cta_output.cta_intersection_point_x_left,
                   cta_instance.core_output.cta_long_intersection[CTA_MODE_REAR][FBK_SIDE_LEFT]);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityX_left,
                   object_data[cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT]].vcs_vel.x);
   // Sign changes because output is in customer coordinate system
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityY_left,
                   -object_data[cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_LEFT]].vcs_vel.y);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_left, cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_LEFT]);

   EXPECT_EQ(cta_output.f_cta_alert_right, FBK_TRUE);
   EXPECT_FLOAT_EQ(cta_output.cta_heading_right, cta_instance.core_output.cta_heading[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
   EXPECT_EQ(cta_output.cta_id_right, cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
   EXPECT_FLOAT_EQ(cta_output.cta_intersection_point_x_right,
                   cta_instance.core_output.cta_long_intersection[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityX_right,
                   object_data[cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]].vcs_vel.x);
   // Sign changes because output is in customer coordinate system
   EXPECT_FLOAT_EQ(cta_output.cta_objVelocityY_right,
                   -object_data[cta_instance.core_output.cta_id[CTA_MODE_REAR][FBK_SIDE_RIGHT]].vcs_vel.y);
   EXPECT_FLOAT_EQ(cta_output.cta_ttc_right, cta_instance.core_output.cta_obj_ttc[CTA_MODE_REAR][FBK_SIDE_RIGHT]);
}

/**
 * Check object corner computation, for an object, that is in the VCS origin with no rotation.
 * \uts{CSCSA-42130} \sdd{SF-3989} \testtype{positive}
 */
TEST_F(Cta_Post_Run_Test, Cta_Calculate_Nissan_Cta_Target_Corners__object_in_vcs_origin_no_rotation)
{
   /** \arrange set cta core output for an object in VCS origin. */
   Fbk_Object_Corners_T target_corners;
   uint8_t approach_side                                                                     = FBK_SIDE_LEFT;
   cta_instance.core_output.cta_index[CTA_MODE_REAR][approach_side]                          = FBK_ZERO_UINT;
   object_data[cta_instance.core_output.cta_index[CTA_MODE_REAR][approach_side]].width       = 2.0f;
   object_data[cta_instance.core_output.cta_index[CTA_MODE_REAR][approach_side]].length      = 5.0f;
   object_data[cta_instance.core_output.cta_index[CTA_MODE_REAR][approach_side]].vcs_heading = 0.0f;
   object_data[cta_instance.core_output.cta_index[CTA_MODE_REAR][approach_side]].vcs_pos.x   = 0.0f;
   object_data[cta_instance.core_output.cta_index[CTA_MODE_REAR][approach_side]].vcs_pos.y   = 0.0f;

   p_vehicle_data->host_length = 4.0f;
   p_vehicle_data->host_width  = 1.5f;

   /** \action execute output setter for cta */
   Cta_Calculate_Nissan_Cta_Target_Corners(&target_corners, &cta_instance, p_vehicle_data, approach_side);

   /** \assert expect that mapping is correctly set */
   EXPECT_FLOAT_EQ(target_corners.points[FBK_FRONT_LEFT_CORNER].x,
                   (Fbk_Half(object_data[cta_instance.core_output.cta_index[CTA_MODE_REAR][approach_side]].length)
                    + p_vehicle_data->host_length));
   EXPECT_FLOAT_EQ(target_corners.points[FBK_FRONT_RIGHT_CORNER].x,
                   (Fbk_Half(object_data[cta_instance.core_output.cta_index[CTA_MODE_REAR][approach_side]].length)
                    + p_vehicle_data->host_length));
   EXPECT_FLOAT_EQ(
      target_corners.points[FBK_FRONT_LEFT_CORNER].y,
      Fbk_Half(object_data[cta_instance.core_output.cta_index[CTA_MODE_REAR][approach_side]].width - p_vehicle_data->host_width));
   EXPECT_FLOAT_EQ(
      target_corners.points[FBK_FRONT_RIGHT_CORNER].y,
      -Fbk_Half(object_data[cta_instance.core_output.cta_index[CTA_MODE_REAR][approach_side]].width + p_vehicle_data->host_width));
}
