#ifndef FBK_ARRAY_INTERPOLATION_TEST_HPP
#define FBK_ARRAY_INTERPOLATION_TEST_HPP

/**
 * @file fbk_array_interpolation.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for FBK array interpolation module.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest.h" // IWYU pragma: keep
#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "pa_reuse.h"
}

/**
 * Class used to create a fixture for array interpolation module test
 * With help of its inherited Setup function the fixture is
 * able to create the same initialization for all of its tests
 */
class Fbk_Array_Interpolation_Test : public ::testing::Test
{
 public:
   const static uint8_t Array_Sizes = 5u;
   float32_T floating_point_test_array[Array_Sizes]{};
   uint8_t uint8_test_array[Array_Sizes]{};

   float32_T float_init_val = 1.0f;
   uint8_t uint_init_val    = 2u;

   virtual void SetUp()
   {
      for (uint8_t idx = 0u; idx < Array_Sizes; idx++)
      {
         if (0u == idx)
         {
            floating_point_test_array[idx] = float_init_val;
            uint8_test_array[idx]          = uint_init_val;
         }
         else
         {
            floating_point_test_array[idx] = floating_point_test_array[idx - 1u] + float_init_val;
            uint8_test_array[idx]          = uint8_test_array[idx - 1u] + uint_init_val;
         }
      }
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }
};

#endif /* FBK_ARRAY_INTERPOLATION_TEST_HPP*/
