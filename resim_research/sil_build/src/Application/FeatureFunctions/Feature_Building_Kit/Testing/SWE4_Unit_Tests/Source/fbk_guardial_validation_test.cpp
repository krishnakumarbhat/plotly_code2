/**
 * @file fbk_guardial_validation_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for FBK Guardial Validation.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-73096}
 */

#include "fbk_guardial_validation_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_guardrail_validation.h"
#include "fbk_iface_types.h"
#include "fbk_index_lookup.h"
#include "fbk_macros.h"
#include "fbk_obj_ageing.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}


/**
 * Tests that guardrail is correctly set.
 * \uts{CSCSA-73097} \sdd{SF-4190} \testtype{positive}
 */
TEST_F(Fbk_Guardrail_Validation_Test, Fbk_Fill_Guardrail_Information__general_test)
{
   /** \arrange set typical context data  */
   Fbk_Guardrail_Data_T guardrail_data_to_set;

   guardrail_data->f_active              = FBK_TRUE;
   guardrail_data->f_present             = FBK_TRUE;
   guardrail_data->status                = PA_OBJ_STATUS_MATURE;
   guardrail_data->lat_pos               = 10.0f;
   guardrail_data->existence_probability = 1.0f;
   guardrail_data->age                   = 3u;

   /** \action fill guardraildata */
   Fbk_Fill_Guardrail_Information(&guardrail_data_to_set, &fbk_context, FBK_SIDE_LEFT);

   /** \assert check if the pointer is not null */
   EXPECT_TRUE(guardrail_data_to_set.f_active);
   EXPECT_TRUE(guardrail_data_to_set.f_present);
   EXPECT_EQ(guardrail_data_to_set.status, PA_OBJ_STATUS_MATURE);
   EXPECT_FLOAT_EQ(guardrail_data_to_set.lat_pos, 10.0f);
   EXPECT_FLOAT_EQ(guardrail_data_to_set.existence_probability, 1.0f);
   EXPECT_EQ(guardrail_data_to_set.age, 3u);
}
