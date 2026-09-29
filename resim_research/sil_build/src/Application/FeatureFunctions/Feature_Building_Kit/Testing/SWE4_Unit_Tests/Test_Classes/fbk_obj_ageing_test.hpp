#ifndef FBK_OBJ_AGEING_TEST_HPP
#define FBK_OBJ_AGEING_TEST_HPP

/**
 * @file fbk_obj_ageing_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test classes for FBK object aging.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest.h" // IWYU pragma: keep
#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_instance.h"
#include "fbk_object_data_t.h"
#include "pa_context.h"
}

/**
 * Class used to create a fixture for fbk_index_lookup module test
 * With help of its inherited Setup function the fixture is
 * able to create the same initialization for all of its tests
 */
class Fbk_Obj_Ageing_Test : public ::testing::Test
{
 public:
   Fbk_Instance_T fbk_instance{};
   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data = nullptr;
   Fbk_Age_Ctr_T &Fbk_Obj_Ages    = fbk_instance.fbk_obj_ages;

   virtual void SetUp()
   {
      /* Initialize context data */
      object_data = data.object_data;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }
};

#endif /* FBK_OBJ_AGEING_TEST_HPP */
