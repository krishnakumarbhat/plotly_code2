#ifndef FBK_MACROS_TEST_HPP
#define FBK_MACROS_TEST_HPP

/**
 * @file fbk_macros_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test classes for FBK macros.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest.h" // IWYU pragma: keep
#include "gtest/gtest_pred_impl.h"

/**
 * Class used to create a fixture for fbk_macros module test
 * With help of its inherited Setup function the fixture is
 * able to create the same initialization for all of its tests
 */
class Fbk_Macros_Test : public ::testing::Test
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

#endif /* FBK_MACROS_TEST_HPP */
