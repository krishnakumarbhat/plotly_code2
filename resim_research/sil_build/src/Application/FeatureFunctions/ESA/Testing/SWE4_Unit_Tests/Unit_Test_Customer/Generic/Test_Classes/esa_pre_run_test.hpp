#ifndef ESA_PRE_RUN_TEST
#define ESA_PRE_RUN_TEST

/**
 * @file esa_pre_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for Generic esa pre run
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

#include "gtest/gtest_pred_impl.h" // IWYU pragma: keep
#include <gtest/gtest.h>           // IWYU pragma: keep

extern "C"
{
#include "esa_core_calibration.h"
#include "esa_core_input_t.h"
#include "esa_input_t.h"
#include "esa_instance_t.h"
#include "fbk_object_data_t.h"
#include "fbk_output.h"
}

/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Esa_Pre_Run_Test : public ::testing::Test
{
 protected:
   Esa_Instance_T esa_instance{};
   Esa_Core_Calibration_T *p_esa_calibration;
   Esa_Core_Input_T *p_esa_core_input;
   Fbk_Output_T fbk_output{};
   Esa_Input_T esa_input{};
   Pa_Data_T pa_data{};

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   void SetUp() override
   {
      fbk_output.p_pa_data = &pa_data;

      p_esa_calibration = &(esa_instance.calibration);
      p_esa_core_input  = &(esa_instance.core_input);

      /* Initialize core input */
      p_esa_core_input->p_pa_data = &pa_data;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /* ESA_PRE_RUN_TEST */
