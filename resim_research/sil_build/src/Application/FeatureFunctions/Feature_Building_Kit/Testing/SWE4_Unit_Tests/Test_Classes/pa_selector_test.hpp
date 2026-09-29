#ifndef PA_SELECTOR_TEST
#define PA_SELECTOR_TEST

/**
 * @file pa_selector_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test classes for PA selector.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest.h" // IWYU pragma: keep
#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "pa_context.h"
}

/**
 * Class used to create a fixture for pa_selector module test
 * With help of its inherited Setup function the fixture is
 * able to create the same initialization for all of its tests
 */
class Pa_Selector_Test : public ::testing::Test
{

 protected:
   Pa_Context_T context{};


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

#endif /* PA_SELECTOR_TEST */
