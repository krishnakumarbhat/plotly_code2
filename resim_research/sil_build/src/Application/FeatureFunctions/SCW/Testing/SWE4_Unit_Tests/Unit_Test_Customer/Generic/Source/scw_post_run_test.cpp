/**
 * @file scw_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for SCW unit tests
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-123974}
 */

#include "scw_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_object_validation.h"
#include "ml_math.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "scw_post_run.c"
}

/*
 * Tests the reset functionality of SCW output. Check that the mapping is done correctly via initialization routine.
 * \uts{CSCSA-123975} \sdd{SF-8187} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Post_Run_Test, Scw_Post_Run_Init__check_no_fatal_failure)
{
   /** \arrange Set up scw output non default values. */
   /** \action Call reset routine. */
   /** \assert Verify that the output is initialized correctly. */
   EXPECT_NO_FATAL_FAILURE(Scw_Post_Run_Init(&scw_instance););
}
/*
 * Tests filling the SCW output for active objects on both sides.
 * \uts{CSCSA-185978} \sdd{SF-8156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Post_Run_Test, Scw_Post_Run__valid_objects_both_sides)
{

   /** \arrange Set up object types. Also SCW state set to ACTIVE */
   p_scw_core_output->obj_type[FBK_SIDE_LEFT]  = SCW_OBJECT_TYPE_DYNAMIC;
   p_scw_core_output->obj_type[FBK_SIDE_RIGHT] = SCW_OBJECT_TYPE_DYNAMIC;

   /** \action Call post run function. */
   Scw_Post_Run(&scw_instance, &scw_input, &scw_output);

   /** \assert Verify that output is filled correctly. */
   EXPECT_EQ(scw_output.scw_object[FBK_SIDE_LEFT].type, SCW_OBJECT_TYPE_DYNAMIC);
   EXPECT_EQ(scw_output.scw_object[FBK_SIDE_RIGHT].type, SCW_OBJECT_TYPE_DYNAMIC);
}

/*
 * Tests filling the SCW output for active guardrails on both sides.
 * \uts{CSCSA-185979} \sdd{SF-8156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Post_Run_Test, Scw_Post_Run__valid_guardrails_both_sides)
{

   /** \arrange Set up object types. Also SCW state set to ACTIVE */
   p_scw_core_output->obj_type[FBK_SIDE_LEFT]  = SCW_OBJECT_TYPE_GUARDRAIL;
   p_scw_core_output->obj_type[FBK_SIDE_RIGHT] = SCW_OBJECT_TYPE_GUARDRAIL;

   /** \action Call post run function. */
   Scw_Post_Run(&scw_instance, &scw_input, &scw_output);

   /** \assert Verify that output is filled correctly. */
   EXPECT_EQ(scw_output.scw_object[FBK_SIDE_LEFT].type, SCW_OBJECT_TYPE_GUARDRAIL);
   EXPECT_EQ(scw_output.scw_object[FBK_SIDE_RIGHT].type, SCW_OBJECT_TYPE_GUARDRAIL);
}


/*
 * Tests the functionality of SCW output. Check that the mapping is done correctly
 * \uts{CSCSA-135511} \sdd{SF-8156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Post_Run_Test, Scw_Post_Run__check_correct_dynamic_object_mapping)
{
   /** \arrange Set up scw core output non default values. */
   Scw_Post_Run_Init(&scw_instance);
   scw_input.f_scw_enable           = FBK_TRUE;
   scw_input.f_scw_enable_dynamic   = FBK_TRUE;
   scw_input.f_scw_enable_guardrail = FBK_TRUE;
   uint8_t side;
   uint8_t obj_index;

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      obj_index                                            = side + FBK_ONE_UINT;
      p_scw_core_output->alert_level[side]                 = SCW_ALERT_LEVEL_1;
      p_scw_core_output->obj_id[side]                      = obj_index;
      p_scw_core_output->obj_index[side]                   = obj_index;
      p_scw_core_output->obj_type[side]                    = SCW_OBJECT_TYPE_DYNAMIC;
      p_scw_core_output->obj_lateral_ttc[side]             = 0.5f;
      p_scw_core_output->obj_lateral_distance[side]        = 2.0f;
      p_scw_core_output->obj_lateral_velocity[side]        = -Fbk_Convert_Obj_Side_To_Sign(side) * 1.0f;
      p_scw_core_output->obj_lateral_acceleration[side]    = 0.1f;
      pa_data.object_data[obj_index].id                    = obj_index;
      pa_data.object_data[obj_index].vcs_pos.x             = -5.0f;
      pa_data.object_data[obj_index].vcs_pos.y             = Fbk_Convert_Obj_Side_To_Sign(side) * 2.0f;
      pa_data.object_data[obj_index].vcs_vel.x             = 10.0f;
      pa_data.object_data[obj_index].vcs_accel.x           = 0.5f;
      pa_data.object_data[obj_index].width                 = 1.8f;
      pa_data.object_data[obj_index].length                = 4.3f;
      pa_data.object_data[obj_index].vcs_heading           = Fbk_Convert_Obj_Side_To_Sign(side) * 0.05f;
      pa_data.object_data[obj_index].existence_probability = 0.99f;
      pa_data.object_data[obj_index].age                   = 5u;
   }

   /** \action Call post run routine. */
   Scw_Post_Run(&scw_instance, &scw_input, &scw_output);

   /** \assert Verify that mapping is done correctly. */
   EXPECT_TRUE(scw_output.f_scw_enabled);
   EXPECT_TRUE(scw_output.f_scw_dyn_enabled);
   EXPECT_TRUE(scw_output.f_scw_guardrail_enabled);
   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      obj_index = side + FBK_ONE_UINT;
      EXPECT_EQ(scw_output.scw_object[side].alert_level, SCW_ALERT_LEVEL_1);
      EXPECT_EQ(scw_output.scw_object[side].id, obj_index);
      EXPECT_EQ(scw_output.scw_object[side].type, SCW_OBJECT_TYPE_DYNAMIC);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].lateral_ttc_s, 0.5f);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].lateral_distance_m, 2.0f);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].position_m.x, -5.0f);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].position_m.y, Fbk_Convert_Obj_Side_To_Sign(side) * 2.0f);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].velocity_mps.x, 10.0f);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].velocity_mps.y, -Fbk_Convert_Obj_Side_To_Sign(side) * 1.0f);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].acceleration_mps2.x, 0.5f);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].acceleration_mps2.y, 0.1f);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].width_m, 1.8f);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].length_m, 4.3f);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].heading_rad, Fbk_Convert_Obj_Side_To_Sign(side) * 0.05f);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].existence_probability, 0.99f);
      EXPECT_EQ(scw_output.scw_object[side].age, 5u);
   }
}


/*
 * Tests the functionality of SCW output. Check that the mapping of critical guardrail data is done correctly
 * \uts{CSCSA-135512} \sdd{SF-8156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Post_Run_Test, Scw_Post_Run__check_correct_guardrail_data_mapping)
{
   /** \arrange Set up scw core output non default values. */
   Scw_Post_Run_Init(&scw_instance);
   scw_input.f_scw_enable           = FBK_TRUE;
   scw_input.f_scw_enable_dynamic   = FBK_TRUE;
   scw_input.f_scw_enable_guardrail = FBK_TRUE;
   uint8_t side;

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      p_scw_core_output->alert_level[side]               = SCW_ALERT_LEVEL_1;
      p_scw_core_output->obj_type[side]                  = SCW_OBJECT_TYPE_GUARDRAIL;
      p_scw_core_output->obj_lateral_ttc[side]           = 0.5f;
      p_scw_core_output->obj_lateral_distance[side]      = 2.0f;
      p_scw_core_output->obj_lateral_velocity[side]      = -Fbk_Convert_Obj_Side_To_Sign(side) * 1.0f;
      p_scw_core_output->obj_lateral_acceleration[side]  = 0.1f;
      pa_data.guardrail_data[side].lat_pos               = Fbk_Convert_Obj_Side_To_Sign(side) * 2.0f;
      pa_data.guardrail_data[side].existence_probability = 0.99f;
      pa_data.guardrail_data[side].age                   = 5u;
   }

   /** \action Call post run routine. */
   Scw_Post_Run(&scw_instance, &scw_input, &scw_output);

   /** \assert Verify that mapping is done correctly. */
   EXPECT_TRUE(scw_output.f_scw_enabled);
   EXPECT_TRUE(scw_output.f_scw_dyn_enabled);
   EXPECT_TRUE(scw_output.f_scw_guardrail_enabled);
   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      EXPECT_EQ(scw_output.scw_object[side].alert_level, SCW_ALERT_LEVEL_1);
      EXPECT_EQ(scw_output.scw_object[side].type, SCW_OBJECT_TYPE_GUARDRAIL);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].lateral_ttc_s, 0.5f);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].lateral_distance_m, 2.0f);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].position_m.x, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].position_m.y, Fbk_Convert_Obj_Side_To_Sign(side) * 2.0f);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].velocity_mps.x, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].velocity_mps.y, -Fbk_Convert_Obj_Side_To_Sign(side) * 1.0f);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].acceleration_mps2.x, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].acceleration_mps2.y, 0.1f);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].width_m, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].length_m, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].heading_rad, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].existence_probability, 0.99f);
      EXPECT_EQ(scw_output.scw_object[side].age, 5u);
   }
}


/*
 * Tests the local reset functionality of SCW output. Check that the mapping is done correctly via initialization routine.
 * \uts{CSCSA-135513} \sdd{CSCSA-125690} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Post_Run_Test, Scw_Reset_Output__check_default_reset)
{
   /** \arrange Set up scw output non default values. */
   scw_output.f_scw_enabled           = FBK_TRUE;
   scw_output.f_scw_dyn_enabled       = FBK_TRUE;
   scw_output.f_scw_guardrail_enabled = FBK_TRUE;
   uint8_t side;

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      scw_output.scw_object[side].alert_level           = SCW_ALERT_LEVEL_1;
      scw_output.scw_object[side].id                    = FBK_ONE_UINT;
      scw_output.scw_object[side].type                  = SCW_OBJECT_TYPE_DYNAMIC;
      scw_output.scw_object[side].lateral_ttc_s         = 0.5f;
      scw_output.scw_object[side].lateral_distance_m    = 2.0f;
      scw_output.scw_object[side].position_m.x          = -5.0f;
      scw_output.scw_object[side].position_m.y          = Fbk_Convert_Obj_Side_To_Sign(side) * 2.0f;
      scw_output.scw_object[side].velocity_mps.x        = 10.0f;
      scw_output.scw_object[side].velocity_mps.y        = 1.0f;
      scw_output.scw_object[side].acceleration_mps2.x   = 0.5f;
      scw_output.scw_object[side].acceleration_mps2.y   = 0.1f;
      scw_output.scw_object[side].width_m               = 1.8f;
      scw_output.scw_object[side].length_m              = 4.3f;
      scw_output.scw_object[side].heading_rad           = 0.05f;
      scw_output.scw_object[side].existence_probability = 0.99f;
      scw_output.scw_object[side].age                   = 5u;
   }

   /** \action Call reset routine. */
   Scw_Reset_Output(&scw_output, p_scw_calibration);

   /** \assert Verify that the output is initialized correctly. */
   EXPECT_FALSE(scw_output.f_scw_enabled);
   EXPECT_FALSE(scw_output.f_scw_dyn_enabled);
   EXPECT_FALSE(scw_output.f_scw_guardrail_enabled);
   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      EXPECT_EQ(scw_output.scw_object[side].alert_level, SCW_NO_ALERT);
      EXPECT_EQ(scw_output.scw_object[side].id, PA_INVALID_OBJ_ID);
      EXPECT_EQ(scw_output.scw_object[side].type, SCW_OBJECT_TYPE_NONE);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].lateral_distance_m, p_scw_calibration->k_scw_lateral_distance_default);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].lateral_ttc_s, p_scw_calibration->k_scw_lateral_ttc_default);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].position_m.x, -SCW_BIG_VALUE);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].position_m.y, Fbk_Convert_Obj_Side_To_Sign(side) * SCW_BIG_VALUE);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].velocity_mps.x, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].velocity_mps.y, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].acceleration_mps2.x, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].acceleration_mps2.y, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].width_m, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].length_m, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].heading_rad, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].existence_probability, FBK_ZERO_F);
      EXPECT_EQ(scw_output.scw_object[side].age, FBK_ZERO_UINT);
   }
}


/*
 * Check that the copying of Scw Core output is done correctly
 * \uts{CSCSA-135514} \sdd{CSCSA-125691} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Post_Run_Test, Scw_Copy_Core_Output__check_correct_copying)
{
   /** \arrange Set up scw core output non default values. */
   Scw_Post_Run_Init(&scw_instance);
   uint8_t side;
   float32_T sign;

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      sign                                     = Fbk_Convert_Obj_Side_To_Sign(side);
      p_scw_core_output->alert_level[side]     = (FBK_SIDE_LEFT == side) ? SCW_ALERT_LEVEL_1 : SCW_ALERT_LEVEL_2;
      p_scw_core_output->obj_id[side]          = side + FBK_ONE_UINT;
      p_scw_core_output->obj_unique_id[side]   = side + FBK_ONE_UINT;
      p_scw_core_output->obj_type[side]        = (FBK_SIDE_LEFT == side) ? SCW_OBJECT_TYPE_DYNAMIC : SCW_OBJECT_TYPE_GUARDRAIL;
      p_scw_core_output->obj_lateral_ttc[side] = 0.5f + sign * 0.1f;
      p_scw_core_output->obj_lateral_distance[side]     = 2.0f + sign * 0.1f;
      p_scw_core_output->obj_lateral_velocity[side]     = -sign * 1.0f;
      p_scw_core_output->obj_lateral_acceleration[side] = 0.1f + sign * 0.01f;
   }

   /** \action Call post run routine. */
   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      Scw_Copy_Core_Output(&scw_output, p_scw_core_output, side);
   }

   /** \assert Verify that mapping is done correctly. */
   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      EXPECT_EQ(scw_output.scw_object[side].alert_level, p_scw_core_output->alert_level[side]);
      EXPECT_EQ(scw_output.scw_object[side].id, p_scw_core_output->obj_id[side]);
      EXPECT_EQ(scw_output.scw_object[side].unique_id, p_scw_core_output->obj_unique_id[side]);
      EXPECT_EQ(scw_output.scw_object[side].type, p_scw_core_output->obj_type[side]);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].lateral_ttc_s, p_scw_core_output->obj_lateral_ttc[side]);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].lateral_distance_m, p_scw_core_output->obj_lateral_distance[side]);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].velocity_mps.y, p_scw_core_output->obj_lateral_velocity[side]);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].acceleration_mps2.y, p_scw_core_output->obj_lateral_acceleration[side]);
   }
}


/*
 * Check that the copying of Scw Core output is not done for invalid side
 * \uts{CSCSA-204201} \sdd{CSCSA-125691} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Post_Run_Test, Scw_Copy_Core_Output__check_no_copying)
{
   /** \arrange Set up scw core output non default values. */
   Scw_Post_Run_Init(&scw_instance);
   uint8_t side;
   float32_T sign;

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      sign                                     = Fbk_Convert_Obj_Side_To_Sign(side);
      p_scw_core_output->alert_level[side]     = (FBK_SIDE_LEFT == side) ? SCW_ALERT_LEVEL_1 : SCW_ALERT_LEVEL_2;
      p_scw_core_output->obj_id[side]          = side + FBK_ONE_UINT;
      p_scw_core_output->obj_unique_id[side]   = side + FBK_ONE_UINT;
      p_scw_core_output->obj_type[side]        = (FBK_SIDE_LEFT == side) ? SCW_OBJECT_TYPE_DYNAMIC : SCW_OBJECT_TYPE_GUARDRAIL;
      p_scw_core_output->obj_lateral_ttc[side] = 0.5f + sign * 0.1f;
      p_scw_core_output->obj_lateral_distance[side]     = 2.0f + sign * 0.1f;
      p_scw_core_output->obj_lateral_velocity[side]     = -sign * 1.0f;
      p_scw_core_output->obj_lateral_acceleration[side] = 0.1f + sign * 0.01f;

      scw_output.scw_object[side].alert_level         = SCW_NO_ALERT;
      scw_output.scw_object[side].id                  = FBK_ZERO_UINT;
      scw_output.scw_object[side].unique_id           = FBK_ZERO_UINT;
      scw_output.scw_object[side].type                = SCW_OBJECT_TYPE_NONE;
      scw_output.scw_object[side].lateral_ttc_s       = SCW_BIG_VALUE;
      scw_output.scw_object[side].lateral_distance_m  = SCW_BIG_VALUE;
      scw_output.scw_object[side].velocity_mps.y      = FBK_ZERO_F;
      scw_output.scw_object[side].acceleration_mps2.y = FBK_ZERO_F;
   }

   side = FBK_NUMBER_OF_SIDES; /* non existing side */

   /** \action Call post run routine. */
   Scw_Copy_Core_Output(&scw_output, p_scw_core_output, side);

   /** \assert Verify that the mapping is not done. */
   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      EXPECT_EQ(scw_output.scw_object[side].alert_level, SCW_NO_ALERT);
      EXPECT_EQ(scw_output.scw_object[side].id, FBK_ZERO_UINT);
      EXPECT_EQ(scw_output.scw_object[side].unique_id, FBK_ZERO_UINT);
      EXPECT_EQ(scw_output.scw_object[side].type, SCW_OBJECT_TYPE_NONE);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].lateral_ttc_s, SCW_BIG_VALUE);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].lateral_distance_m, SCW_BIG_VALUE);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].velocity_mps.y, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].acceleration_mps2.y, FBK_ZERO_F);
   }
}


/*
 * Check that the critical dynamic object data mapping is done correctly
 * \uts{CSCSA-135515} \sdd{CSCSA-125692} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Post_Run_Test, Scw_Set_Dynamic_Object_Output__check_correct_mapping)
{
   /** \arrange Set up scw core output non default values. */
   Scw_Post_Run_Init(&scw_instance);
   uint8_t side;
   uint8_t obj_index;
   float32_T sign;

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      obj_index                                            = side + FBK_ONE_UINT;
      sign                                                 = Fbk_Convert_Obj_Side_To_Sign(side);
      p_scw_core_output->obj_index[side]                   = obj_index;
      pa_data.object_data[obj_index].id                    = obj_index;
      pa_data.object_data[obj_index].vcs_pos.x             = -5.0f + sign * 0.1f;
      pa_data.object_data[obj_index].vcs_pos.y             = sign * 2.0f;
      pa_data.object_data[obj_index].vcs_vel.x             = 10.0f + sign * 0.1f;
      pa_data.object_data[obj_index].vcs_accel.x           = 0.5f + sign * 0.1f;
      pa_data.object_data[obj_index].width                 = 1.8f + sign * 0.1f;
      pa_data.object_data[obj_index].length                = 4.3f + sign * 0.1f;
      pa_data.object_data[obj_index].vcs_heading           = sign * 0.05f;
      pa_data.object_data[obj_index].existence_probability = 0.9f + sign * 0.1f;
      pa_data.object_data[obj_index].age                   = (FBK_SIDE_LEFT == side) ? 5u : 4u;
   }

   /** \action Call post run routine. */
   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      Scw_Set_Dynamic_Object_Output(&scw_output, &pa_data, p_scw_core_output, side);
   }

   /** \assert Verify that mapping is done correctly. */
   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      obj_index = side + FBK_ONE_UINT;
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].position_m.x, pa_data.object_data[obj_index].vcs_pos.x);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].position_m.y, pa_data.object_data[obj_index].vcs_pos.y);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].velocity_mps.x, pa_data.object_data[obj_index].vcs_vel.x);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].acceleration_mps2.x, pa_data.object_data[obj_index].vcs_accel.x);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].width_m, pa_data.object_data[obj_index].width);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].length_m, pa_data.object_data[obj_index].length);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].heading_rad, pa_data.object_data[obj_index].vcs_heading);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].existence_probability, pa_data.object_data[obj_index].existence_probability);
      EXPECT_EQ(scw_output.scw_object[side].age, pa_data.object_data[obj_index].age);
   }
}


/*
 * Check that the critical dynamic object data mapping is not done for invalid side
 * \uts{CSCSA-204202} \sdd{CSCSA-125692} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Post_Run_Test, Scw_Set_Dynamic_Object_Output__check_no_mapping)
{
   /** \arrange Set up scw core output non default values. */
   Scw_Post_Run_Init(&scw_instance);
   uint8_t side;
   uint8_t obj_index;
   float32_T sign;

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      obj_index                                            = side + FBK_ONE_UINT;
      sign                                                 = Fbk_Convert_Obj_Side_To_Sign(side);
      p_scw_core_output->obj_index[side]                   = obj_index;
      pa_data.object_data[obj_index].id                    = obj_index;
      pa_data.object_data[obj_index].vcs_pos.x             = -5.0f + sign * 0.1f;
      pa_data.object_data[obj_index].vcs_pos.y             = sign * 2.0f;
      pa_data.object_data[obj_index].vcs_vel.x             = 10.0f + sign * 0.1f;
      pa_data.object_data[obj_index].vcs_accel.x           = 0.5f + sign * 0.1f;
      pa_data.object_data[obj_index].width                 = 1.8f + sign * 0.1f;
      pa_data.object_data[obj_index].length                = 4.3f + sign * 0.1f;
      pa_data.object_data[obj_index].vcs_heading           = sign * 0.05f;
      pa_data.object_data[obj_index].existence_probability = 0.9f + sign * 0.1f;
      pa_data.object_data[obj_index].age                   = (FBK_SIDE_LEFT == side) ? 5u : 4u;

      scw_output.scw_object[side].position_m.x          = SCW_BIG_VALUE;
      scw_output.scw_object[side].position_m.y          = SCW_BIG_VALUE;
      scw_output.scw_object[side].velocity_mps.x        = FBK_ZERO_F;
      scw_output.scw_object[side].acceleration_mps2.x   = FBK_ZERO_F;
      scw_output.scw_object[side].width_m               = FBK_ZERO_F;
      scw_output.scw_object[side].length_m              = FBK_ZERO_F;
      scw_output.scw_object[side].heading_rad           = FBK_ZERO_F;
      scw_output.scw_object[side].existence_probability = FBK_ZERO_F;
      scw_output.scw_object[side].age                   = FBK_ZERO_UINT;
   }

   side = FBK_NUMBER_OF_SIDES; /* non existing value */

   /** \action Call post run routine. */
   Scw_Set_Dynamic_Object_Output(&scw_output, &pa_data, p_scw_core_output, side);

   /** \assert Verify that mapping is not done. */
   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].position_m.x, SCW_BIG_VALUE);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].position_m.y, SCW_BIG_VALUE);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].velocity_mps.x, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].acceleration_mps2.x, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].width_m, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].length_m, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].heading_rad, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].existence_probability, FBK_ZERO_F);
      EXPECT_EQ(scw_output.scw_object[side].age, FBK_ZERO_UINT);
   }
}


/*
 * Check that the critical guardrail data mapping is done correctly
 * \uts{CSCSA-135516} \sdd{CSCSA-125693} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Post_Run_Test, Scw_Set_Guardrail_Output__check_correct_mapping)
{
   /** \arrange Set up scw core output non default values. */
   Scw_Post_Run_Init(&scw_instance);
   uint8_t side;
   float32_T sign;

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      sign                                               = Fbk_Convert_Obj_Side_To_Sign(side);
      pa_data.guardrail_data[side].lat_pos               = sign * 2.0f;
      pa_data.guardrail_data[side].existence_probability = 0.9f + sign * 0.1f;
      pa_data.guardrail_data[side].age                   = (FBK_SIDE_LEFT == side) ? 5u : 4u;
   }

   /** \action Call post run routine. */
   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      Scw_Set_Guardrail_Output(&scw_output, &pa_data, p_scw_core_output, side);
   }

   /** \assert Verify that mapping is done correctly. */
   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].position_m.y, pa_data.guardrail_data[side].lat_pos);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].existence_probability, pa_data.guardrail_data[side].existence_probability);
      EXPECT_EQ(scw_output.scw_object[side].age, pa_data.guardrail_data[side].age);
   }
}


/*
 * Check that the critical guardrail data mapping is not done for invalid side
 * \uts{CSCSA-204203} \sdd{CSCSA-125693} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Post_Run_Test, Scw_Set_Guardrail_Output__check_no_mapping)
{
   /** \arrange Set up scw core output non default values. */
   Scw_Post_Run_Init(&scw_instance);
   uint8_t side;
   float32_T sign;

   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      sign                                               = Fbk_Convert_Obj_Side_To_Sign(side);
      pa_data.guardrail_data[side].lat_pos               = sign * 2.0f;
      pa_data.guardrail_data[side].existence_probability = 0.9f + sign * 0.1f;
      pa_data.guardrail_data[side].age                   = (FBK_SIDE_LEFT == side) ? 5u : 4u;

      scw_output.scw_object[side].position_m.y          = SCW_BIG_VALUE;
      scw_output.scw_object[side].existence_probability = FBK_ZERO_F;
      scw_output.scw_object[side].age                   = FBK_ZERO_UINT;
   }

   side = FBK_NUMBER_OF_SIDES; /* non existing value */

   /** \action Call post run routine. */
   Scw_Set_Guardrail_Output(&scw_output, &pa_data, p_scw_core_output, side);

   /** \assert Verify that mapping is not done. */
   for (side = FBK_ZERO_UINT; side < FBK_NUMBER_OF_SIDES; side++)
   {
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].position_m.y, SCW_BIG_VALUE);
      EXPECT_FLOAT_EQ(scw_output.scw_object[side].existence_probability, FBK_ZERO_F);
      EXPECT_EQ(scw_output.scw_object[side].age, FBK_ZERO_UINT);
   }
}


#ifndef NDEBUG
/*
 * Tests the functionality of SCW output when the output pointer is null
 * \uts{CSCSA-123977} \sdd{SF-8156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Post_Run_Test, Scw_Post_Run__null_instance_pointer)
{
   /** \arrange */
   /** \action Call post run routine. */
   /** \assert Verify that an assert is thrown, if Scw_Post_Run function is called with NULL output pointer. */
   EXPECT_DEATH({ Scw_Post_Run(NULL, &scw_input, &scw_output); }, ".*p_scw_instance.*");
}


/*
 * Tests the functionality of SCW output when the core output pointer is null
 * \uts{CSCSA-123978} \sdd{SF-8156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Post_Run_Test, Scw_Post_Run__null_p_scw_input_pointer)
{
   /** \arrange */
   /** \action Call post run routine. */
   /** \assert Verify that an assert is thrown, if Scw_Post_Run function is called with NULL core output pointer. */
   EXPECT_DEATH({ Scw_Post_Run(&scw_instance, NULL, &scw_output); }, ".*p_scw_input.*");
}


/*
 * Tests the functionality of SCW output when the input pointer is null
 * \uts{CSCSA-123980} \sdd{SF-8156} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Post_Run_Test, Scw_Post_Run__null_p_scw_output_pointer)
{
   /** \arrange */
   /** \action Call post run routine. */
   /** \assert Verify that an assert is thrown, if Scw_Post_Run function is called with NULL input pointer. */
   EXPECT_DEATH({ Scw_Post_Run(&scw_instance, &scw_input, NULL); }, ".*p_scw_output.*");
}

/*
 * Tests the reset functionality of SCW output when the output pointer is null
 * \uts{CSCSA-123979} \sdd{SF-8187} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Post_Run_Test, Scw_Post_Run_Init__null_output_pointer)
{
   /** \arrange */
   /** \action Call init routine. */
   /** \assert Check that exception is thrown. */
   EXPECT_DEATH({ Scw_Post_Run_Init(NULL); }, ".*p_scw_instance.*");
}
#endif