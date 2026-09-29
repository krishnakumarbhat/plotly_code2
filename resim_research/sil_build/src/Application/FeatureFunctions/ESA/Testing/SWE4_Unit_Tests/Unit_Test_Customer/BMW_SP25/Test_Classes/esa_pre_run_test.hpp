#ifndef ESA_PRE_RUN_TEST_HPP
#define ESA_PRE_RUN_TEST_HPP

/**
 * @file esa_pre_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for ESA bmw sp25 pre run unit tests
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "esa_core_calibration.h"
#include "esa_core_input_t.h"
#include "esa_core_output_t.h"
#include "esa_iface.h"
#include "esa_instance_t.h"
#include "esa_output_t.h"
#include "esa_persistent_t.h"
#include "fbk_guardrail_data_t.h"
#include "fbk_output.h"
}

/**
 * Class used to create a fixture for ESA bmw sp25 pre run tests
 * For writing two or more tests that operate on similar data, we can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Esa_Pre_Run_Test : public ::testing::Test
{
 protected:
   Esa_Instance_T esa_instance{};
   Esa_Persistent_T *p_esa_persistent;
   Esa_Core_Calibration_T *p_esa_calibration;
   Esa_Core_Input_T *p_esa_core_input;
   Esa_Core_Output_T *p_esa_core_output;
   Fbk_Output_T fbk_output{};
   Esa_Input_T esa_input{};
   Esa_Output_T esa_output{};
   Pa_Data_T pa_data{};
   Fbk_Vehicle_Data_T *p_vehicle_data;
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   void SetUp() override
   {
      fbk_output.p_pa_data = &pa_data;

      p_esa_persistent  = &(esa_instance.persistent);
      p_esa_calibration = &(esa_instance.calibration);
      p_esa_core_input  = &(esa_instance.core_input);
      p_esa_core_output = &(esa_instance.core_output);

      p_vehicle_data = &(pa_data.vehicle_data);

      /* Initialize core input */
      p_esa_core_input->p_pa_data = &pa_data;

      /* Initialize calibration values */
      Esa_Core_Cal_Update_Defaults(p_esa_calibration);

      p_vehicle_data->host_width  = 2.0f;
      p_vehicle_data->host_length = 5.0f;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /* ESA_PRE_RUN_TEST_HPP*/
