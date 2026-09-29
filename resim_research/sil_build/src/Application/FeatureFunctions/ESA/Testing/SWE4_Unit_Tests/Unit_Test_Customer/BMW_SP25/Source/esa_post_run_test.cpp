/**
 * @file esa_post_run_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for ESA unit tests
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{}
 */

#include "esa_post_run_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "esa_post_run.c"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "pa_reuse.h"
}


/*
 * Tests filling the ESA output for active objects on both sides.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Esa_Post_Run_Test, Esa_Post_Run__valid_objects_both_sides)
{
   /** \arrange Set up alerts */
   p_esa_core_output->esa_core_status           = ESA_CORE_STATUS_ACTIVE;
   p_esa_core_output->esa_alert[FBK_SIDE_LEFT]  = FBK_TRUE;
   p_esa_core_output->esa_alert[FBK_SIDE_RIGHT] = FBK_TRUE;
   p_esa_core_output->esa_id[FBK_SIDE_LEFT]     = 25u;
   p_esa_core_output->esa_id[FBK_SIDE_RIGHT]    = 36u;

   /** \action Call post run function. */
   Esa_Post_Run(&esa_instance, &esa_input, &esa_output);

   /** \assert Verify that output is filled correctly. */
   EXPECT_EQ(esa_output.Object_ID_Left, 25u);
   EXPECT_EQ(esa_output.Object_ID_Right, 36u);
}


/*
 * Tests the reset functionality of ESA output. Check that the mapping is done correctly.
 * \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Esa_Post_Run_Test, Esa_Reset_Output__check_default_reset)
{
   /** \arrange Non applicable. */

   esa_output.Esa_Status_Left = ESA_STATUS_VEHICLE_IN_ZONE;
   /* Reset object properties on the left side of ego. */
   esa_output.Object_Existence_Probability_Left = 0.5f;
   esa_output.Object_ID_Left                    = 42u;
   esa_output.Object_Length_Left                = 42.0f;
   esa_output.Object_Position_X_Left            = 42.0f;
   esa_output.Object_Position_Y_Left            = 42.0f;
   esa_output.Object_Speed_X_Left               = 42.0f;
   esa_output.Object_Speed_Y_Left               = 42.0f;
   esa_output.Object_Timestamp_Left             = 1000.0f;
   esa_output.Object_Time_To_Pass_Left          = 42.0f;
   esa_output.Object_Width_Left                 = 42.0f;

   esa_output.Esa_Status_Right = ESA_STATUS_VEHICLE_IN_ZONE;
   /* Reset object properties on the right side of ego. */
   esa_output.Object_Existence_Probability_Right = 0.5f;
   esa_output.Object_ID_Right                    = 42u;
   esa_output.Object_Length_Right                = 42.0f;
   esa_output.Object_Position_X_Right            = 42.0f;
   esa_output.Object_Position_Y_Right            = 42.0f;
   esa_output.Object_Speed_X_Right               = 42.0f;
   esa_output.Object_Speed_Y_Right               = 42.0f;
   esa_output.Object_Timestamp_Right             = 1000.0f;
   esa_output.Object_Time_To_Pass_Right          = 42.0f;
   esa_output.Object_Width_Right                 = 42.0f;

   /** \action Call reset routine. */
   Esa_Reset_Output(&esa_output);

   /** \assert Verify that mapping is done correctly. */
   EXPECT_EQ(esa_output.Esa_Status_Left, ESA_STATUS_NO_VEHICLE_IN_ZONE);
   EXPECT_FLOAT_EQ(esa_output.Object_Existence_Probability_Left, FBK_ZERO_F);
   EXPECT_EQ(esa_output.Object_ID_Left, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(esa_output.Object_Length_Left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(esa_output.Object_Position_X_Left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(esa_output.Object_Position_Y_Left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(esa_output.Object_Speed_X_Left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(esa_output.Object_Speed_Y_Left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(esa_output.Object_Timestamp_Left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(esa_output.Object_Time_To_Pass_Left, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(esa_output.Object_Width_Left, FBK_ZERO_F);
   /*Right Object Information*/
   EXPECT_EQ(esa_output.Esa_Status_Right, ESA_STATUS_NO_VEHICLE_IN_ZONE);
   EXPECT_FLOAT_EQ(esa_output.Object_Existence_Probability_Right, FBK_ZERO_F);
   EXPECT_EQ(esa_output.Object_ID_Right, FBK_ZERO_UINT);
   EXPECT_FLOAT_EQ(esa_output.Object_Length_Right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(esa_output.Object_Position_X_Right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(esa_output.Object_Position_Y_Right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(esa_output.Object_Speed_X_Right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(esa_output.Object_Speed_Y_Right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(esa_output.Object_Timestamp_Right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(esa_output.Object_Time_To_Pass_Right, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(esa_output.Object_Width_Right, FBK_ZERO_F);
}

#ifndef NDEBUG
/*
 * Tests the init functionality of ESA output.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Esa_Post_Run_Test, Esa_Post_Run_Init__null_instance_pointer)
{
   /** \arrange Non applicable. */
   /** \action Non applicable. */
   /** \assert Verify that ESA throws an exception if instance pointer is null. */
   EXPECT_DEATH({ Esa_Post_Run_Init(NULL); }, ".*p_esa_instance.*");
}
#endif //! NDEBUG