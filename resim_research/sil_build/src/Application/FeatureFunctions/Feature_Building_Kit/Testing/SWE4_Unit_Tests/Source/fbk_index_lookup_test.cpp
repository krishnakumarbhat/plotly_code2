/**
 * @file fbk_index_lookup_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for FBK index lookup table.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42233}
 */

#include "fbk_index_lookup_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_index_lookup.c"
#include "fbk_object_data_t.h"
#include "pa_const_macros.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}

using ::testing::Each;
using ::testing::Eq;

/**
 * Check if Fbk_Get_Object_Index_From_Id returns the correct index value.
 * \uts{CSCSA-42397} \sdd{SF-4153} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Index_Lookup_Test, Fbk_Get_Object_Index_From_Id__return_correct_index_for_given_id)
{
   /** \arrange Set up an object index and id pair in the lookup table. */
   uint8_t object_index = 10;
   uint8_t object_id    = PA_OBJ_NUMBER_OF_OBJECTS - 1u;
   Fbk_Reset_Index_Id_Lookup_Table(&Fbk_Index_Id_Lookup_Table);

   Fbk_Index_Id_Lookup_Table.lookup_table[object_id] = object_index;

   /** \action Run the function to return the index value. */
   uint8_t returned_object_index = Fbk_Get_Object_Index_From_Id(&Fbk_Index_Id_Lookup_Table, object_id);

   /** \assert Check if returned value matches. */
   EXPECT_EQ(object_index, returned_object_index);
}

/**
 * Check if Fbk_Get_Object_Index_From_Id returns an invalid index value for an invalid index value.
 * \uts{CSCSA-42398} \sdd{SF-4153} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Index_Lookup_Test, Fbk_Get_Object_Index_From_Id__test_invalid_object_id)
{
   /** \arrange Set up an object index and id pair in the lookup table. */
   uint8_t object_id = PA_INVALID_OBJ_ID;
   Fbk_Reset_Index_Id_Lookup_Table(&Fbk_Index_Id_Lookup_Table);

   /** \action Run the function to return the index value. */
   uint8_t returned_object_index = Fbk_Get_Object_Index_From_Id(&Fbk_Index_Id_Lookup_Table, object_id);

   /** \assert Check if returned value matches. */
   EXPECT_EQ(returned_object_index, PA_INVALID_OBJ_INDEX);
}

/**
 * Check if Fbk_Get_Object_Index_From_Id returns an invalid index value for an index value larger than PA_OBJ_NUMBER_OF_OBJECTS.
 * \uts{CSCSA-42399} \sdd{SF-4153} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Index_Lookup_Test, Fbk_Get_Object_Index_From_Id__test_object_id_out_of_bounds)
{
   /** \arrange Set up an object index and id pair in the lookup table. */
   uint8_t object_id = PA_OBJ_NUMBER_OF_OBJECTS + FBK_ONE_UINT;
   Fbk_Reset_Index_Id_Lookup_Table(&Fbk_Index_Id_Lookup_Table);

   /** \action Run the function to return the index value. */
   uint8_t returned_object_index = Fbk_Get_Object_Index_From_Id(&Fbk_Index_Id_Lookup_Table, object_id);

   /** \assert Check if returned value matches. */
   EXPECT_EQ(returned_object_index, PA_INVALID_OBJ_INDEX);
}

/**
 * Check if boundaries are respected from the outputs of feature building kit. Here one specific output is out of bound.
 * \uts{CSCSA-42400} \sdd{SF-4157} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Index_Lookup_Test, Fbk_Is_Id_Index_Lut_In_Boundaries__value_out_of_bound)
{
   /** \arrange Set up internals to defaults. */
   boolean_T res;
   Fbk_Reset_Index_Id_Lookup_Table(&Fbk_Index_Id_Lookup_Table);

   Fbk_Index_Id_Lookup_Table.lookup_table[3] = PA_OBJ_NUMBER_OF_OBJECTS + 3u;

   /** \action executes function to test */
   res = Fbk_Is_Id_Index_Lut_In_Boundaries(&Fbk_Index_Id_Lookup_Table);

   /** \assert Expect false since boundaries are not kept. */
   EXPECT_FALSE(res);
}


/**
 * Check if Fbk_Update_Index_Id_Lookup_Table sets the correct index values at the correct array entry.
 * \uts{CSCSA-42401} \sdd{SF-4154} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Index_Lookup_Test, Fbk_Update_Index_Id_Lookup_Table__set_correct_index_values_in_lut)
{
   /** \arrange Set up an object index and id pair in the tracker output. */
   uint8_t object_index         = 10;
   uint8_t object_id            = PA_OBJ_NUMBER_OF_OBJECTS - 10u;
   object_data[object_index].id = object_id;

   /** \action Run the function to populate the lookup table. */
   Fbk_Update_Index_Id_Lookup_Table(&Fbk_Index_Id_Lookup_Table, &pa_data);

   /** \assert Check if the lookup table is filled correctly. */
   EXPECT_EQ(Fbk_Index_Id_Lookup_Table.lookup_table[object_id], object_index);
   EXPECT_EQ(Fbk_Index_Id_Lookup_Table.lookup_table[0], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(Fbk_Index_Id_Lookup_Table.lookup_table[PA_OBJ_NUMBER_OF_OBJECTS], PA_INVALID_OBJ_INDEX);
}

/**
 * Check if Fbk_Update_Index_Id_Lookup_Table sets the correct index values at the correct array entry.
 * \uts{CSCSA-42402} \sdd{SF-4154} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Index_Lookup_Test, Fbk_Update_Index_Id_Lookup_Table__set_invalid_index_for_invalid_object_id)
{
   /** \arrange Set up an object with invalid id in the tracker output. */
   uint8_t object_index             = 10;
   uint8_t object_id                = PA_INVALID_OBJ_ID;
   object_data[object_index].id     = object_id;
   object_data[object_index].status = PA_OBJ_STATUS_MATURE;

   /** \action Run the function to populate the lookup table. */
   Fbk_Update_Index_Id_Lookup_Table(&Fbk_Index_Id_Lookup_Table, &pa_data);

   /** \assert Check if the lookup table is filled correctly. */
   EXPECT_THAT(Fbk_Index_Id_Lookup_Table.lookup_table, Each(Eq(PA_INVALID_OBJ_INDEX)));
}

/**
 * Check if Fbk_Update_Index_Id_Lookup_Table sets the correct index values at the correct array entry.
 * \uts{CSCSA-42403} \sdd{SF-4154} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Index_Lookup_Test, Fbk_Update_Index_Id_Lookup_Table__set_invalid_index_for_object_id_out_of_bounds)
{
   /** \arrange Set up an invalid id in the tracker output. */
   uint8_t object_index             = 10;
   uint8_t object_id                = PA_OBJ_NUMBER_OF_OBJECTS + FBK_ONE_UINT;
   object_data[object_index].id     = object_id;
   object_data[object_index].status = PA_OBJ_STATUS_MATURE;

   /** \action Run the function to populate the lookup table. */
   Fbk_Update_Index_Id_Lookup_Table(&Fbk_Index_Id_Lookup_Table, &pa_data);

   /** \assert Check if the lookup table is filled correctly. */
   EXPECT_THAT(Fbk_Index_Id_Lookup_Table.lookup_table, Each(Eq(PA_INVALID_OBJ_INDEX)));
}


/**
 * Check if Fbk_Reset_Index_Id_Lookup_Table resets the lookup table properly.
 * \uts{CSCSA-42404} \sdd{SF-4155} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Index_Lookup_Test, Fbk_Reset_Index_Id_Lookup_Table__works_properly)
{
   /** \arrange Set up some non-default entries in lookup table. */
   Fbk_Index_Id_Lookup_Table.lookup_table[0u]                       = 12u;
   Fbk_Index_Id_Lookup_Table.lookup_table[5u]                       = 25u;
   Fbk_Index_Id_Lookup_Table.lookup_table[PA_OBJ_NUMBER_OF_OBJECTS] = 2u;

   /** \action Run the function to reset the lookup table. */
   Fbk_Reset_Index_Id_Lookup_Table(&Fbk_Index_Id_Lookup_Table);

   /** \assert Check if the lookup table is reseted correctly. */
   EXPECT_EQ(Fbk_Index_Id_Lookup_Table.lookup_table[0u], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(Fbk_Index_Id_Lookup_Table.lookup_table[5u], PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(Fbk_Index_Id_Lookup_Table.lookup_table[PA_OBJ_NUMBER_OF_OBJECTS], PA_INVALID_OBJ_INDEX);
}

#ifndef NDEBUG

/**
 * Check if Fbk_Update_Index_Id_Lookup_Table is throwing an exception when input is NULL.
 * \uts{CSCSA-42405} \sdd{SF-4154} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Index_Lookup_Test, Fbk_Update_Index_Id_Lookup_Table__context_is_null)
{
   EXPECT_DEATH(
      {
         /** \arrange set up NULL pointer */
         /** \action call function to test */
         Fbk_Update_Index_Id_Lookup_Table(&Fbk_Index_Id_Lookup_Table, NULL);

         /** \assert check that assertion is thrown */
      },
      ".*p_pa_data.*");
}
#endif