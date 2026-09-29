#ifndef FBK_IFACE_TEST_HPP
#define FBK_IFACE_TEST_HPP

/**
 * @file fbk_iface_test.hpp
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
#include "fbk_instance.h"
#include "fbk_output.h"
#include "pa_context.h"
}

/**
 * Class used to create a fixture for fbk_index_lookup module test
 * With help of its inherited Setup function the fixture is
 * able to create the same initialization for all of its tests
 */
class Fbk_Iface_Test : public ::testing::Test
{
 public:
   Fbk_Instance_T fbk_instance{};
   Pa_Context_T fbk_context;
   Fbk_Output_T fbk_output;
   Pa_Data_T pa_data{};

   virtual void SetUp()
   {
      fbk_context.p_data = &pa_data;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }
};

#endif /* FBK_IFACE_TEST_HPP */
