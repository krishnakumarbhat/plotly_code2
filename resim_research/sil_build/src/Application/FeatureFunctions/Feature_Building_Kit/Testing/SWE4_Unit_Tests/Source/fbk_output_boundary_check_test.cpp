/**
 * @file fbk_output_boundary_check_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for FBK output boundary checks.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */
/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{WI-21693}
 */

#include "fbk_output_boundary_check_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_iface_types.h"
#include "fbk_index_lookup.h"
#include "fbk_macros.h"
#include "fbk_obj_ageing.h"
#include "fbk_output_boundary_check.c"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}

/**
 * Check if boundaries are respected from the outputs of feature building kit.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Fbk_Output_Boundary_Check_Test, Fbk_Are_Outputs_In_Boundary__boundaries_are_kept)
{
   /** \arrange Set up internals to defaults.*/
   boolean_T res;
   Fbk_Reset_Index_Id_Lookup_Table(&fbk_index_id_lookup_table);
   Fbk_Reset_Object_Ageing(&age_counter);

   /** \action executes function to test */
   res = Fbk_Are_Outputs_In_Boundary(&fbk_index_id_lookup_table, &age_counter);

   /** \assert Expect boundaries to be respected.*/
   EXPECT_TRUE(res);
}


/**
 * Check if boundaries are respected from the outputs of feature building kit. Here a stage age is set to a value which is outside
 * of the allowed boundaries. Thus false shall be returned. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Fbk_Output_Boundary_Check_Test, Fbk_Are_Outputs_In_Boundary__boundaries_are_not_adhered)
{
   /** \arrange Set up internals to defaults and one output to something undefined.*/
   boolean_T res;
   Fbk_Reset_Index_Id_Lookup_Table(&fbk_index_id_lookup_table);
   Fbk_Reset_Object_Ageing(&age_counter);

   age_counter.stage_age[1] = FBK_ZERO_UINT;

   /** \action executes function to test */
   res = Fbk_Are_Outputs_In_Boundary(&fbk_index_id_lookup_table, &age_counter);

   /** \assert Expect false since a stage age is modified to an unreachable value.*/
   EXPECT_FALSE(res);
}


/**
 * Check if boundaries are respected from the outputs of feature building kit. Here the boundary of the index id lut are kept, thus
 * true is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Fbk_Output_Boundary_Check_Test, Fbk_Is_Id_Index_Lut_In_Boundaries__boundaries_are_kept)
{
   /** \arrange Set up internals to defaults.*/
   boolean_T res;
   Fbk_Reset_Index_Id_Lookup_Table(&fbk_index_id_lookup_table);

   /** \action executes function to test */
   res = Fbk_Is_Id_Index_Lut_In_Boundaries(&fbk_index_id_lookup_table);

   /** \assert Expect true since boundaries are kept.*/
   EXPECT_TRUE(res);
}


/**
 * Check if boundaries are respected from the outputs of feature building kit. Here the boundary of object ageing properties are
 * kept, thus true is expected. \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Fbk_Output_Boundary_Check_Test, Fbk_Is_Object_Ageing_Output_In_Boundaries__boundaries_are_kept)
{
   /** \arrange Set up internals to defaults.*/
   boolean_T res;
   Fbk_Reset_Object_Ageing(&age_counter);

   /** \action executes function to test */
   res = Fbk_Is_Object_Ageing_Output_In_Boundaries(&age_counter);

   /** \assert Expect true since boundaries are kept.*/
   EXPECT_TRUE(res);
}


/**
 * Check if boundaries are respected from the outputs of feature building kit. Here the boundary of object ageing properties are
 * not kept, thus false is expected. \uts{} \sdd{} \testtype{negative}
 */
TEST_F(Fbk_Output_Boundary_Check_Test, Fbk_Is_Object_Ageing_Output_In_Boundaries__boundaries_are_not_adhered_by_stage_age)
{
   /** \arrange Set up internals to defaults.*/
   boolean_T res;
   Fbk_Reset_Object_Ageing(&age_counter);
   age_counter.stage_age[0] = FBK_ZERO_UINT;

   /** \action executes function to test */
   res = Fbk_Is_Object_Ageing_Output_In_Boundaries(&age_counter);

   /** \assert Expect false since boundaries are not adhered.*/
   EXPECT_FALSE(res);
}
