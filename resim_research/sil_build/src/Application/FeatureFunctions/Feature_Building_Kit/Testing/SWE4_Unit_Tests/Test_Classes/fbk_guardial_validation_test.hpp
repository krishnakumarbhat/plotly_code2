#ifndef FBK_GUARDRAIL_VALIDATION_TEST_HPP
#define FBK_GUARDRAIL_VALIDATION_TEST_HPP

/**
 * @file fbk_guardial_validation_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test classes for FBK index lookup table.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest.h" // IWYU pragma: keep
#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "pa_context.h"
}

/**
 * Class used to create a fixture for fbk_guardial_validation module test
 * With help of its inherited Setup function the fixture is
 * able to create the same initialization for all of its tests
 */
class Fbk_Guardrail_Validation_Test : public ::testing::Test
{

 public:
   Pa_Context_T fbk_context{};
   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Guardrail_Data_T *guardrail_data;

   virtual void SetUp()
   {
      /* Initialize context data */
      fbk_context.p_data = &data;
      object_data        = fbk_context.p_data->object_data;
      guardrail_data     = fbk_context.p_data->guardrail_data;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }
};

#endif /* FBK_GUARDRAIL_VALIDATION_TEST_HPP */
