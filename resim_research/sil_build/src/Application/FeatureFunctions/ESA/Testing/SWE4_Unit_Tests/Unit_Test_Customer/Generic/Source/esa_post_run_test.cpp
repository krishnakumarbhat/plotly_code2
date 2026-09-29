/**
 * @file esa_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for Generic ESA post run
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-136079}
 */

#include "esa_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "esa_post_run.c"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "ml_math.h"
#include "pa_reuse.h"
}


/**
 * Check that post run initialization routine resets the generic output accordingly. Intialization via Esa_Post_Run_Init function
 * \uts{CSCSA-136080} \sdd{CSCSA-66565} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Post_Run_Test, Esa_Post_Run_Init__check_initialization_routine)
{
   /** \arrange set generic esa output to non default */
   /** \action Run Esa_Post_Run_Init to test */
   /** \assert Verify generic esa output is set to default */
   EXPECT_NO_FATAL_FAILURE(Esa_Post_Run_Init(&esa_instance););
}

/**
 * Check that post run initialization routine resets the generic output accordingly. initialization via Esa_Reset_Output function
 * \uts{CSCSA-136081} \sdd{CSCSA-123054} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Post_Run_Test, Esa_Reset_Output__check_initialization_routine)
{
   uint8_t side_index;
   uint8_t obj_index_left  = 11u;
   uint8_t obj_id_left     = 13u;
   uint8_t obj_index_right = 21u;
   uint8_t obj_id_right    = 23u;

   /** \arrange set generic esa output to non default */
   esa_output.esa_status = ESA_CORE_STATUS_DEACTIVATED_LOW_CURVE_RADIUS;

   esa_output.f_esa_alert[FBK_SIDE_LEFT] = FBK_TRUE;

   esa_output.esa_object[FBK_SIDE_LEFT].id                             = obj_id_left;
   esa_output.esa_object[FBK_SIDE_LEFT].index                          = obj_index_left;
   esa_output.esa_object[FBK_SIDE_LEFT].width_m                        = 1.1f;
   esa_output.esa_object[FBK_SIDE_LEFT].length_m                       = 1.2f;
   esa_output.esa_object[FBK_SIDE_LEFT].long_pos_m                     = 1.3f;
   esa_output.esa_object[FBK_SIDE_LEFT].lat_pos_m                      = 1.4f;
   esa_output.esa_object[FBK_SIDE_LEFT].long_speed_mps                 = 1.5f;
   esa_output.esa_object[FBK_SIDE_LEFT].lat_speed_mps                  = 1.6f;
   esa_output.esa_object[FBK_SIDE_LEFT].ttc_s                          = 1.7f;
   esa_output.esa_object[FBK_SIDE_LEFT].ttp_s                          = 1.8f;
   esa_output.esa_object[FBK_SIDE_LEFT].decel_to_reach_host_speed_mps2 = 1.9f;
   esa_output.esa_object[FBK_SIDE_LEFT].long_distance_m                = 1.21f;
   esa_output.esa_object[FBK_SIDE_LEFT].existence_prob                 = 1.22f;

   esa_output.f_esa_alert[FBK_SIDE_RIGHT] = FBK_TRUE;

   esa_output.esa_object[FBK_SIDE_RIGHT].id                             = obj_id_right;
   esa_output.esa_object[FBK_SIDE_RIGHT].index                          = obj_index_right;
   esa_output.esa_object[FBK_SIDE_RIGHT].width_m                        = 2.1f;
   esa_output.esa_object[FBK_SIDE_RIGHT].length_m                       = 2.2f;
   esa_output.esa_object[FBK_SIDE_RIGHT].long_pos_m                     = 2.3f;
   esa_output.esa_object[FBK_SIDE_RIGHT].lat_pos_m                      = 2.4f;
   esa_output.esa_object[FBK_SIDE_RIGHT].long_speed_mps                 = 2.5f;
   esa_output.esa_object[FBK_SIDE_RIGHT].lat_speed_mps                  = 2.6f;
   esa_output.esa_object[FBK_SIDE_RIGHT].ttc_s                          = 2.7f;
   esa_output.esa_object[FBK_SIDE_RIGHT].ttp_s                          = 2.8f;
   esa_output.esa_object[FBK_SIDE_RIGHT].decel_to_reach_host_speed_mps2 = 2.9f;
   esa_output.esa_object[FBK_SIDE_RIGHT].long_distance_m                = 2.21f;
   esa_output.esa_object[FBK_SIDE_RIGHT].existence_prob                 = 2.22f;

   /** \action Run Esa_Reset_Output to test */
   Esa_Reset_Output(&esa_output);

   /** \assert Verify generic esa output is set to default */
   EXPECT_EQ(esa_output.esa_status, ESA_CORE_STATUS_DISABLED_BY_INPUT);

   for (side_index = FBK_ZERO_UINT; side_index < FBK_NUMBER_OF_SIDES; side_index++)
   {
      EXPECT_FALSE(esa_output.f_esa_alert[side_index]);

      EXPECT_EQ(esa_output.esa_object[side_index].id, PA_INVALID_OBJ_ID);
      EXPECT_EQ(esa_output.esa_object[side_index].index, PA_INVALID_OBJ_INDEX);

      EXPECT_FLOAT_EQ(esa_output.esa_object[side_index].width_m, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(esa_output.esa_object[side_index].length_m, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(esa_output.esa_object[side_index].long_pos_m, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(esa_output.esa_object[side_index].lat_pos_m, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(esa_output.esa_object[side_index].long_speed_mps, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(esa_output.esa_object[side_index].lat_speed_mps, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(esa_output.esa_object[side_index].ttc_s, ESA_DEFAULT_LARGE_TTC);
      EXPECT_FLOAT_EQ(esa_output.esa_object[side_index].ttp_s, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(esa_output.esa_object[side_index].decel_to_reach_host_speed_mps2, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(esa_output.esa_object[side_index].long_distance_m, -ESA_DEFAULT_OBJ_DIST);
      EXPECT_FLOAT_EQ(esa_output.esa_object[side_index].existence_prob, FBK_ZERO_F);
   }
}


/**
 * Check that post run match core output to generic output accordingly. Matching via Esa_Post_Run function
 * \uts{CSCSA-136082} \sdd{CSCSA-66566} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Post_Run_Test, Esa_Post_Run__core_to_generic_match)
{
   uint8_t obj_index_left  = 11u;
   uint8_t obj_id_left     = 13u;
   uint8_t obj_index_right = 21u;
   uint8_t obj_id_right    = 23u;

   /** \arrange set generic esa output to custom values */

   p_esa_core_output->esa_core_status = ESA_CORE_STATUS_ACTIVE;

   // Core and tracker data, LEFT SIDE
   p_esa_core_output->esa_alert[FBK_SIDE_LEFT] = FBK_TRUE;

   p_esa_core_output->esa_index[FBK_SIDE_LEFT]                     = obj_index_left;
   p_esa_core_output->esa_id[FBK_SIDE_LEFT]                        = obj_id_left;
   object_data[obj_index_left].id                                  = obj_id_left;
   object_data[obj_index_left].index                               = obj_index_left;
   object_data[obj_index_left].width                               = 1.1f;
   object_data[obj_index_left].length                              = 1.2f;
   object_data[obj_index_left].curvi_pos.x                         = 1.3f;
   object_data[obj_index_left].curvi_pos.y                         = 1.4f;
   object_data[obj_index_left].curvi_vel.x                         = 1.5f;
   object_data[obj_index_left].curvi_vel.y                         = 1.6f;
   p_esa_core_output->esa_ttc[FBK_SIDE_LEFT]                       = 1.7f;
   p_esa_core_output->esa_ttp[FBK_SIDE_LEFT]                       = 1.8f;
   p_esa_core_output->esa_decel_to_reach_host_speed[FBK_SIDE_LEFT] = 1.9f;
   p_esa_core_output->esa_long_distance[FBK_SIDE_LEFT]             = 1.11f;
   object_data[obj_index_left].existence_probability               = 0.5;

   // Core and tracker data, RIGHT SIDE
   p_esa_core_output->esa_alert[FBK_SIDE_LEFT] = FBK_FALSE;

   p_esa_core_output->esa_index[FBK_SIDE_RIGHT]                     = obj_index_right;
   p_esa_core_output->esa_id[FBK_SIDE_RIGHT]                        = obj_id_right;
   object_data[obj_index_right].id                                  = obj_id_right;
   object_data[obj_index_right].index                               = obj_index_right;
   object_data[obj_index_right].width                               = 2.1f;
   object_data[obj_index_right].length                              = 2.2f;
   object_data[obj_index_right].curvi_pos.x                         = 2.3f;
   object_data[obj_index_right].curvi_pos.y                         = 2.4f;
   object_data[obj_index_right].curvi_vel.x                         = 2.5f;
   object_data[obj_index_right].curvi_vel.y                         = 2.6f;
   p_esa_core_output->esa_ttc[FBK_SIDE_RIGHT]                       = 2.7f;
   p_esa_core_output->esa_ttp[FBK_SIDE_RIGHT]                       = 2.8f;
   p_esa_core_output->esa_decel_to_reach_host_speed[FBK_SIDE_RIGHT] = 2.9f;
   p_esa_core_output->esa_long_distance[FBK_SIDE_RIGHT]             = 2.11f;
   object_data[obj_index_right].existence_probability               = 0.7f;

   /** \action Run Esa_Post_Run to match signals */
   Esa_Post_Run(&esa_instance, &esa_input, &esa_output);

   /** \assert Verify generic esa output is set to defined values */

   esa_output.esa_status = ESA_CORE_STATUS_ACTIVE;

   // Generic output, LEFT SIDE
   esa_output.f_esa_alert[FBK_SIDE_LEFT] = FBK_TRUE;

   EXPECT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].id, obj_id_left);
   EXPECT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].index, obj_index_left);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].width_m, 1.1f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].length_m, 1.2f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].long_pos_m, 1.3f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].lat_pos_m, 1.4f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].long_speed_mps, 1.5f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].lat_speed_mps, 1.6f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].ttc_s, 1.7f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].ttp_s, 1.8f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].decel_to_reach_host_speed_mps2, 1.9f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].long_distance_m, 1.11f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].existence_prob, 0.5f);

   // Generic output, RIGHT SIDE
   esa_output.f_esa_alert[FBK_SIDE_RIGHT] = FBK_FALSE;

   EXPECT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].id, obj_id_right);
   EXPECT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].index, obj_index_right);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].width_m, 2.1f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].length_m, 2.2f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].long_pos_m, 2.3f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].lat_pos_m, 2.4f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].long_speed_mps, 2.5f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].lat_speed_mps, 2.6f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].ttc_s, 2.7f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].ttp_s, 2.8f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].decel_to_reach_host_speed_mps2, 2.9f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].long_distance_m, 2.11f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].existence_prob, 0.7f);
}

/**
 * Check that post run match core output to generic output accordingly. Matching via Esa_Update_Output function
 * \uts{CSCSA-136083} \sdd{CSCSA-123055} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Post_Run_Test, Esa_Update_Output__core_to_generic_match)
{
   uint8_t obj_index_left  = 11u;
   uint8_t obj_id_left     = 13u;
   uint8_t obj_index_right = 21u;
   uint8_t obj_id_right    = 23u;

   /** \arrange set generic esa output to custom values */

   p_esa_core_output->esa_core_status = ESA_CORE_STATUS_ACTIVE;

   p_esa_core_output->esa_alert[FBK_SIDE_LEFT] = FBK_TRUE;

   p_esa_core_output->esa_index[FBK_SIDE_LEFT]                     = obj_index_left;
   p_esa_core_output->esa_id[FBK_SIDE_LEFT]                        = obj_id_left;
   object_data[obj_index_left].id                                  = obj_id_left;
   object_data[obj_index_left].index                               = obj_index_left;
   object_data[obj_index_left].width                               = 1.1f;
   object_data[obj_index_left].length                              = 1.2f;
   object_data[obj_index_left].curvi_pos.x                         = 1.3f;
   object_data[obj_index_left].curvi_pos.y                         = 1.4f;
   object_data[obj_index_left].curvi_vel.x                         = 1.5f;
   object_data[obj_index_left].curvi_vel.y                         = 1.6f;
   p_esa_core_output->esa_ttc[FBK_SIDE_LEFT]                       = 1.7f;
   p_esa_core_output->esa_ttp[FBK_SIDE_LEFT]                       = 1.8f;
   p_esa_core_output->esa_decel_to_reach_host_speed[FBK_SIDE_LEFT] = 1.9f;
   p_esa_core_output->esa_long_distance[FBK_SIDE_LEFT]             = 1.11f;
   object_data[obj_index_left].existence_probability               = 0.5;

   p_esa_core_output->esa_alert[FBK_SIDE_LEFT] = FBK_FALSE;

   p_esa_core_output->esa_index[FBK_SIDE_RIGHT]                     = obj_index_right;
   p_esa_core_output->esa_id[FBK_SIDE_RIGHT]                        = obj_id_right;
   object_data[obj_index_right].id                                  = obj_id_right;
   object_data[obj_index_right].index                               = obj_index_right;
   object_data[obj_index_right].width                               = 2.1f;
   object_data[obj_index_right].length                              = 2.2f;
   object_data[obj_index_right].curvi_pos.x                         = 2.3f;
   object_data[obj_index_right].curvi_pos.y                         = 2.4f;
   object_data[obj_index_right].curvi_vel.x                         = 2.5f;
   object_data[obj_index_right].curvi_vel.y                         = 2.6f;
   p_esa_core_output->esa_ttc[FBK_SIDE_RIGHT]                       = 2.7f;
   p_esa_core_output->esa_ttp[FBK_SIDE_RIGHT]                       = 2.8f;
   p_esa_core_output->esa_decel_to_reach_host_speed[FBK_SIDE_RIGHT] = 2.9f;
   p_esa_core_output->esa_long_distance[FBK_SIDE_RIGHT]             = 2.11f;
   object_data[obj_index_right].existence_probability               = 0.7f;

   /** \action Run Esa_Update_Output to match signals */
   Esa_Update_Output(&esa_output, &esa_instance);

   /** \assert Verify generic esa output is set to defined values */

   esa_output.esa_status = ESA_CORE_STATUS_ACTIVE;

   esa_output.f_esa_alert[FBK_SIDE_LEFT] = FBK_TRUE;

   EXPECT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].id, obj_id_left);
   EXPECT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].index, obj_index_left);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].width_m, 1.1f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].length_m, 1.2f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].long_pos_m, 1.3f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].lat_pos_m, 1.4f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].long_speed_mps, 1.5f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].lat_speed_mps, 1.6f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].ttc_s, 1.7f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].ttp_s, 1.8f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].decel_to_reach_host_speed_mps2, 1.9f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].long_distance_m, 1.11f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_LEFT].existence_prob, 0.5f);

   esa_output.f_esa_alert[FBK_SIDE_RIGHT] = FBK_FALSE;

   EXPECT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].id, obj_id_right);
   EXPECT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].index, obj_index_right);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].width_m, 2.1f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].length_m, 2.2f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].long_pos_m, 2.3f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].lat_pos_m, 2.4f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].long_speed_mps, 2.5f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].lat_speed_mps, 2.6f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].ttc_s, 2.7f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].ttp_s, 2.8f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].decel_to_reach_host_speed_mps2, 2.9f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].long_distance_m, 2.11f);
   EXPECT_FLOAT_EQ(esa_output.esa_object[FBK_SIDE_RIGHT].existence_prob, 0.7f);
}

/*
 * Tests filling the ESA output for active objects on both sides.
 * \uts{CSCSA-185762} \sdd{CSCSA-66566} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Post_Run_Test, Esa_Post_Run__valid_objects_both_sides)
{

   /** \arrange Set up object types. Also ESA state set to ACTIVE */
   p_esa_core_output->esa_alert[FBK_SIDE_LEFT]  = FBK_TRUE;
   p_esa_core_output->esa_alert[FBK_SIDE_RIGHT] = FBK_TRUE;

   /** \action Call post run function. */
   Esa_Post_Run(&esa_instance, &esa_input, &esa_output);

   /** \assert Verify that output is filled correctly. */
   EXPECT_TRUE(esa_output.f_esa_alert[FBK_SIDE_LEFT]);
   EXPECT_TRUE(esa_output.f_esa_alert[FBK_SIDE_RIGHT]);
}

#ifndef NDEBUG
/*
 * Tests the init functionality of ESA output.
 * \uts{CSCSA-185757} \sdd{CSCSA-66565} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Post_Run_Test, Esa_Post_Run_Init__null_output_pointer)
{
   /** \arrange */
   /** \action Call init routine. */
   /** \assert Check that exception is thrown. */
   EXPECT_DEATH({ Esa_Post_Run_Init(NULL); }, ".*p_esa_instance.*");
}
#endif