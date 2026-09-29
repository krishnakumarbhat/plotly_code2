/**
 * @file cta_struct_initializer_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for cta_struct_initializer.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42039}
 */

#include "cta_struct_initializer_test.hpp"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "cta_struct_initializer.c"
#include "cta_types.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
}

/**
 * Test the reset function of cta core output Expect the cta core output to be reset to its default.
 * \uts{CSCSA-42040} \sdd{SF-3882} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Struct_Initializer_Test, Cta_Reset_Core_Output__reset_core_output_to_default_vals)
{
   /** \arrange values unequal to the default ones */

   uint8_t mode                                    = CTA_MODE_REAR;
   cta_core_output.cta_status                      = CTA_STATUS_ACTIVE;
   cta_core_output.f_cta_enabled                   = FBK_TRUE;
   cta_core_output.cta_index[mode][FBK_SIDE_LEFT]  = 2;
   cta_core_output.cta_index[mode][FBK_SIDE_RIGHT] = 5;


   /** \action executes function to test */
   Cta_Reset_Core_Output(&cta_core_output);

   /** \assert Expect core output to be reset */
   EXPECT_EQ(cta_core_output.cta_status, CTA_STATUS_DISABLED);
   EXPECT_EQ(cta_core_output.cta_index[mode][FBK_SIDE_LEFT], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(cta_core_output.cta_index[mode][FBK_SIDE_RIGHT], PA_INVALID_OBJ_INDEX);
}

#ifndef NDEBUG
/**
 * Test the reset function of cta core output with a pointer to NULL Expect death of this function.
 * \uts{CSCSA-42041} \sdd{SF-3882} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Struct_Initializer_Test, Cta_Reset_Core_Output__core_output_null)
{
   EXPECT_DEATH(
      {
         /** \arrange nothing to be arranged here */
         /** \action executes function to test */
         Cta_Reset_Core_Output(NULL);
         /** \assert Expect core output to be reset */
      },
      ".*p_cta_core_output.*");
}
#endif

/**
 * Test the reset function of cta core persistent data Expect the cta core persistent data to be reset to its default.
 * \uts{CSCSA-42042} \sdd{SF-3883} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Struct_Initializer_Test, Cta_Reset_Persistent__reset_persistent_to_default_vals)
{
   /** \arrange values unequal to the default ones */
   uint8_t side_idx;
   uint8_t mode;
   for (mode = FBK_ZERO_UINT; mode < CTA_NUM_MODES; mode++)
   {
      for (side_idx = FBK_ZERO_UINT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
      {
         cta_persistent.previous_brake_qualifier[mode][side_idx]             = FBK_TRUE;
         cta_persistent.brake_suppression_counter[mode][side_idx]            = FBK_ONE_UINT;
         cta_persistent.warning_holding_counter[mode][side_idx]              = FBK_ONE_UINT;
         cta_persistent.brake_holding_counter[mode][side_idx]                = FBK_ONE_UINT;
         cta_persistent.previous_most_critical_obj_id[mode][side_idx]        = FBK_ONE_UINT;
         cta_persistent.previous_most_critical_unique_obj_id[mode][side_idx] = FBK_ONE_UINT;
         cta_persistent.previous_crit_level[mode][side_idx]                  = CTA_CRIT_LEVEL_1;
      }
   }

   /** \action executes function to test */
   Cta_Reset_Persistent(&cta_persistent);

   /** \assert Expect core persistent to be reset */
   for (mode = FBK_ZERO_UINT; mode < CTA_NUM_MODES; mode++)
   {
      for (side_idx = FBK_ZERO_UINT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
      {
         EXPECT_FALSE(cta_persistent.previous_brake_qualifier[mode][side_idx]);
         EXPECT_EQ(cta_persistent.brake_suppression_counter[mode][side_idx], FBK_ZERO_UINT);
         EXPECT_EQ(cta_persistent.warning_holding_counter[mode][side_idx], FBK_ZERO_UINT);
         EXPECT_EQ(cta_persistent.brake_holding_counter[mode][side_idx], FBK_ZERO_UINT);
         EXPECT_EQ(cta_persistent.previous_most_critical_obj_id[mode][side_idx], FBK_ZERO_UINT);
         EXPECT_EQ(cta_persistent.previous_most_critical_unique_obj_id[mode][side_idx], FBK_ZERO_UINT);
         EXPECT_EQ(cta_persistent.previous_crit_level[mode][side_idx], CTA_CRIT_LEVEL_NONE);
      }
   }
}