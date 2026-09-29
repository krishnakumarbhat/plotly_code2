#ifndef RECW_PRE_RUN_TEST_HPP
#define RECW_PRE_RUN_TEST_HPP

/**
 * @file recw_itv_simulation_features_test_hpp.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for Rivian SRR6 RECW pre run unit tests
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_guardrail_data_t.h"
#include "fbk_output.h"
#include "pa_data.h"
#include "recw_core_calibration.h"
#include "recw_core_input_t.h"
#include "recw_input_t.h"
#include "recw_instance.h"
}

/**
 * Class used to create a fixture for BMW SRR5 pre run
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Recw_Pre_Run_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Recw_Input_T recw_input{};
   Recw_Core_Calibration_T cals;
   Recw_Core_Input_T recw_core_input{};
   Pa_Data_T data{};
   Fbk_Guardrail_Data_T *guardrail_data;
   Fbk_Output_T fbk_output{};
   Recw_Instance_T recw_instance{};

   void SetUp() override
   {
      /* Initialize calibration values */
      Recw_Core_Cal_Update_Defaults(&cals);
      /* Initialize context data */
      guardrail_data       = data.guardrail_data;
      fbk_output.p_pa_data = &data;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /* RECW_PRE_RUN_TEST_HPP */
