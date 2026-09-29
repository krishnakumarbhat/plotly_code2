#ifndef FBK_INDEX_LOOKUP_TEST_HPP
#define FBK_INDEX_LOOKUP_TEST_HPP

/**
 * @file fbk_index_lookup_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test classes for FBK index lookup table.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest.h" // IWYU pragma: keep
#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_index_lookup.h"
#include "fbk_object_data_t.h"
#include "pa_context.h"
}

/**
 * Class used to create a fixture for fbk_index_lookup module test
 * With help of its inherited Setup function the fixture is
 * able to create the same initialization for all of its tests
 */
class Fbk_Index_Lookup_Test : public ::testing::Test
{

 public:
   Pa_Data_T pa_data{};
   Fbk_Object_Data_T *const object_data = pa_data.object_data;
   Fbk_Index_Id_Lookup_Table_T Fbk_Index_Id_Lookup_Table;

   virtual void SetUp()
   {
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }
};

#endif /* FBK_INDEX_LOOKUP_TEST_HPP */
