#ifndef SCW_IFACE_TEST_HPP
#define SCW_IFACE_TEST_HPP

/**
 * @file scw_iface_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for SCW unit tests
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_output.h"
#include "scw_core_calibration.h"
#include "scw_iface.h" // IWYU pragma: keep
#include "scw_instance_t.h"
}

/**
 * Class used to create a fixture for SCW Tests
 * For writing two or more tests that operate on similar data, we can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Scw_Iface_Test : public ::testing::Test
{
 public:
   Scw_Instance_T scw_instance{};
   Scw_Core_Calibration_T *p_scw_calibration;
   Fbk_Output_T fbk_output;
   Pa_Data_T pa_data{};
   Scw_Input_T scw_input{};
   Scw_Output_T scw_output{};

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   void SetUp() override
   {
      fbk_output.p_pa_data              = &pa_data;
      scw_instance.core_input.p_pa_data = &pa_data;
      p_scw_calibration                 = &(scw_instance.calibration);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /* SCW_IFACE_TEST_HPP */
