#ifndef FBK_FUNCTIONS_TEST_HPP
#define FBK_FUNCTIONS_TEST_HPP

/**
 * @file fbk_functions_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test classes for FBK swap.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest.h" // IWYU pragma: keep
#include "gtest/gtest_pred_impl.h"

/**
 * Class used to create a fixture for fbk_functions module test
 * With help of its inherited Setup function the fixture is
 * able to create the same initialization for all of its tests
 */
class Fbk_Functions_Test : public ::testing::Test
{

 public:
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

#endif /* FBK_FUNCTIONS_TEST_HPP */
