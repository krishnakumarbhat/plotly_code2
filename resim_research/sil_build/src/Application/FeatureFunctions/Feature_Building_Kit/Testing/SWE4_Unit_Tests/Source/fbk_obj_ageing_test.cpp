/**
 * @file fbk_obj_ageing_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for FBK object aging.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42239}
 */

#include "fbk_obj_ageing_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_iface_types.h"
#include "fbk_macros.h"
#include "fbk_obj_ageing.c"
#include "fbk_object_data_t.h"
#include "pa_const_macros.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}

/**
 * Tests whether age counting properties are incremented correctly. Here default values are expected.
 * \uts{CSCSA-42451} \sdd{SF-4150} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Obj_Ageing_Test, Fbk_Reset_Object_Ageing__Reset_To_Default)
{
   /** \arrange Initialize structure with non default values */
   for (uint8_t idx = FBK_ZERO_UINT; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      Fbk_Obj_Ages.stage[idx]     = PA_OBJ_STATUS_MATURE;
      Fbk_Obj_Ages.stage_age[idx] = 20u;
   }

   /** \action executes function to test. */
   Fbk_Reset_Object_Ageing(&Fbk_Obj_Ages);

   /** \assert expect default values to be used. */
   for (uint8_t idx = FBK_ZERO_UINT; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      EXPECT_EQ(Fbk_Obj_Ages.stage[idx], PA_OBJ_STATUS_INVALID);
      EXPECT_EQ(Fbk_Obj_Ages.stage_age[idx], FBK_ONE_UINT);
   }
}


/**
 * Tests whether stage age is reset correctly when all object are changing their stage.
 * \uts{CSCSA-42452} \sdd{SF-4152} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Obj_Ageing_Test, Fbk_Update_Object_Ageing__Return_correct_pointer)
{
   /** \arrange Initialize structure with non default values */
   for (uint8_t idx = FBK_ZERO_UINT; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      Fbk_Obj_Ages.stage[idx]     = PA_OBJ_STATUS_INVALID;
      object_data[idx].status     = PA_OBJ_STATUS_NEW;
      Fbk_Obj_Ages.stage_age[idx] = 20u;
   }

   /** \action executes function to test. */
   Fbk_Update_Object_Ageing(&Fbk_Obj_Ages, &data);

   /** \assert expect default values to be used. */
   for (uint8_t idx = FBK_ZERO_UINT; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      EXPECT_EQ(Fbk_Obj_Ages.stage[idx], PA_OBJ_STATUS_NEW);
      EXPECT_EQ(Fbk_Obj_Ages.stage_age[idx], FBK_ONE_UINT);
   }
}


/**
 * Tests whether stage age is kept when all object are changing their stage to implausible coasting.
 * \uts{CSCSA-42453} \sdd{SF-4152} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Obj_Ageing_Test, Fbk_Update_Object_Ageing__Each_Object_Status_Is_Implausible)
{
   /** \arrange Initialize objects with mature status in previous cycle and in the current cycle with implausible coasting behavior
    */
   uint8_t initial_stage_age = 5u;
   for (uint8_t idx = FBK_ZERO_UINT; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      Fbk_Obj_Ages.stage[idx]                = PA_OBJ_STATUS_MATURE;
      Fbk_Obj_Ages.stage_age[idx]            = initial_stage_age;
      object_data[idx].status                = PA_OBJ_STATUS_COASTED;
      object_data[idx].f_is_in_rr_sensor_fov = FBK_FALSE;
      object_data[idx].f_is_in_rl_sensor_fov = FBK_FALSE;
      object_data[idx].f_is_in_fr_sensor_fov = FBK_FALSE;
      object_data[idx].f_is_in_fl_sensor_fov = FBK_FALSE;
   }

   /** \action executes function to test. */
   Fbk_Update_Object_Ageing(&Fbk_Obj_Ages, &data);

   /** \assert expect stage age to remain and implausible coasting to be set as state. */
   for (uint8_t idx = FBK_ZERO_UINT; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      EXPECT_EQ(Fbk_Obj_Ages.stage[idx], PA_OBJ_STATUS_COASTED_IMPLAUSIBLE);
      EXPECT_EQ(Fbk_Obj_Ages.stage_age[idx], initial_stage_age);
   }
}


/**
 * Tests whether stage age is reset correctly when all object are changing their stage.
 * \uts{CSCSA-42454} \sdd{SF-4152} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Obj_Ageing_Test, Fbk_Update_Object_Ageing__Each_Object_Status_Changes_From_Implausible_To_Mature)
{
   /** \arrange Initialize objects with implausible coasting status in previous cycle and in the current cycle with mature behavior
    */
   uint8_t initial_stage_age = 5u;
   for (uint8_t idx = FBK_ZERO_UINT; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      Fbk_Obj_Ages.stage[idx]                = PA_OBJ_STATUS_COASTED_IMPLAUSIBLE;
      Fbk_Obj_Ages.stage_age[idx]            = initial_stage_age;
      object_data[idx].status                = PA_OBJ_STATUS_MATURE;
      object_data[idx].f_is_in_rr_sensor_fov = FBK_FALSE;
      object_data[idx].f_is_in_rl_sensor_fov = FBK_FALSE;
      object_data[idx].f_is_in_fr_sensor_fov = FBK_FALSE;
      object_data[idx].f_is_in_fl_sensor_fov = FBK_FALSE;
   }

   /** \action executes function to test. */
   Fbk_Update_Object_Ageing(&Fbk_Obj_Ages, &data);

   /** \assert expect stage age to remain and implausible coasting to be set as state. */
   for (uint8_t idx = FBK_ZERO_UINT; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      EXPECT_EQ(Fbk_Obj_Ages.stage[idx], PA_OBJ_STATUS_MATURE);
      EXPECT_EQ(Fbk_Obj_Ages.stage_age[idx], initial_stage_age + 1u);
   }
}


#ifndef NDEBUG

/**
 * Check if Fbk_Update_Object_Ageing is throwing an exception when input is NULL.
 * \uts{CSCSA-42455} \sdd{SF-4152} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Obj_Ageing_Test, Fbk_Update_Object_Ageing__context_is_null)
{
   EXPECT_DEATH(
      {
         /** \arrange set up NULL pointer */
         /** \action call function to test */
         Fbk_Update_Object_Ageing(&Fbk_Obj_Ages, NULL);

         /** \assert check that assertion is thrown */
      },
      ".*pa_data.*");
}
#endif