#ifndef PT_CONSTANTS_TEST_HPP
#define PT_CONSTANTS_TEST_HPP
/**
 * @file pt_constants_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the test fixture class for pt_constants of customer BMW.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "gtest/gtest_pred_impl.h"

/**
 * Class used to create a fixture for pt_constants.c
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Pt_Constants_Test : public ::testing::Test
{
 public:
   void SetUp() override
   {
   }


   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

 protected:
};


#endif