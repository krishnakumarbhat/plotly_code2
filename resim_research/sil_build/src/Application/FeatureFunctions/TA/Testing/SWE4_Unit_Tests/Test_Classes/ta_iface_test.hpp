#ifndef TA_IFACE_TEST_HPP
#define TA_IFACE_TEST_HPP

/**
 * @file ta_iface_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for TA unit tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "ta_input_t.h"
#include <gtest/gtest_pred_impl.h>

extern "C"
{
#include "fbk_output.h"
#include "pa_data.h"
#include "ta_input_t.h"
#include "ta_instance_t.h"
#include "ta_output_t.h"
}

/**
 * Class used to create a fixture for TA test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ta_Iface_Test : public ::testing::Test
{
 public:
   Ta_Instance_T ta_instance{};
   Ta_Input_T ta_input{};
   Ta_Output_T ta_output{};
   Pa_Data_T data{};
   Fbk_Output_T fbk_output{};
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   void SetUp() override
   {
      ta_instance.core_input.p_pa_data = &data;
      fbk_output.p_pa_data             = &data;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};
#endif /*TA_IFACE_TEST_HPP*/
