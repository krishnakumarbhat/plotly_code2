#ifndef ESA_IFACE_TEST_HPP
#define ESA_IFACE_TEST_HPP

/**
 * @file esa_iface_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class Lcda_Iface_Test
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

//#include "gtest/gtest_pred_impl.h"
#include "gtest/gtest.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "esa_core_calibration.h"
#include "esa_iface.h"
#include "esa_instance_t.h"
#include "esa_output_t.h"
#include "fbk_output.h"
}

/**
 * Class used to create a fixture for esa_tests.c
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Esa_Iface_Test : public ::testing::Test
{
 public:
   Esa_Instance_T esa_instance{};
   Esa_Core_Calibration_T *p_esa_calibration;
   Fbk_Output_T fbk_output;
   Pa_Data_T pa_data{};
   Esa_Input_T esa_input{};
   Esa_Output_T esa_output{};

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   void SetUp() override
   {
      fbk_output.p_pa_data              = &pa_data;
      esa_instance.core_input.p_pa_data = &pa_data;
      p_esa_calibration                 = &(esa_instance.calibration);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /* ESA_IFACE_TEST_HPP */
