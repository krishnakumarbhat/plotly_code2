#ifndef SCW_HELPER_FUNCTIONS_TEST_HPP
#define SCW_HELPER_FUNCTIONS_TEST_HPP

/**
 * @file scw_helper_functions_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for SCW unit tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "scw_types.h"
}

/**
 * Class used to create a fixture for SCW Tests
 * For writing two or more tests that operate on similar data, we can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Scw_Helper_Functions_Test : public ::testing::Test
{
 protected:
   Scw_Object_T scw_tracker_obj{};
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   void SetUp() override
   {
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};


#endif /* SCW_HELPER_FUNCTIONS_TEST_HPP */
