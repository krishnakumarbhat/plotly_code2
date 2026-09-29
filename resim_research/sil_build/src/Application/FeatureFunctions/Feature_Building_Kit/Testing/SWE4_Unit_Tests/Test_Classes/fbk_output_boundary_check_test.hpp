#ifndef FBK_OUTPUT_BOUNDARY_CHECK_TEST_HPP
#define FBK_OUTPUT_BOUNDARY_CHECK_TEST_HPP

/**
 * @file fbk_output_boundary_check_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test classe for output boundary check.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest.h" // IWYU pragma: keep
#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_iface_types.h"
#include "fbk_index_lookup.h"
#include "fbk_obj_ageing.h"
}

/**
 * Class used to create a fixture for fbk_macros module test
 * With help of its inherited Setup function the fixture is
 * able to create the same initialization for all of its tests
 */
class Fbk_Output_Boundary_Check_Test : public ::testing::Test
{

 public:
   Fbk_Age_Ctr_T age_counter;
   Fbk_Index_Id_Lookup_Table_T fbk_index_id_lookup_table;

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

#endif /* FBK_OUTPUT_BOUNDARY_CHECK_TEST_HPP */
