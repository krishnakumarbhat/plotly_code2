#ifndef SCW_PRE_RUN_TEST_HPP
#define SCW_PRE_RUN_TEST_HPP

/**
 * @file scw_pre_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for SCW bmw sp25 pre run unit tests
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_guardrail_data_t.h"
#include "fbk_output.h"
#include "pa_context.h"
#include "scw_core_calibration.h"
#include "scw_core_input_t.h"
#include "scw_core_output_t.h"
#include "scw_iface.h"
#include "scw_instance_t.h"
#include "scw_output_t.h"
#include "scw_persistent_t.h"
}

/**
 * Class used to create a fixture for SCW bmw sp25 pre run tests
 * For writing two or more tests that operate on similar data, we can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Scw_Pre_Run_Test : public ::testing::Test
{
 protected:
   Scw_Instance_T scw_instance{};
   Scw_Persistent_T *p_scw_persistent;
   Scw_Core_Calibration_T *p_scw_calibration;
   Scw_Core_Input_T *p_scw_core_input;
   Scw_Core_Output_T *p_scw_core_output;
   Fbk_Output_T fbk_output{};
   Scw_Input_T scw_input{};
   Scw_Output_T scw_output{};
   Pa_Data_T pa_data{};
   Fbk_Guardrail_Data_T *guardrail_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   void SetUp() override
   {
      fbk_output.p_pa_data = &pa_data;

      p_scw_persistent  = &(scw_instance.persistent);
      p_scw_calibration = &(scw_instance.calibration);
      p_scw_core_input  = &(scw_instance.core_input);
      p_scw_core_output = &(scw_instance.core_output);

      guardrail_data = pa_data.guardrail_data;
      p_vehicle_data = &(pa_data.vehicle_data);

      /* Initialize core input */
      p_scw_core_input->p_pa_data = &pa_data;

      /* Initialize calibration values */
      Scw_Core_Cal_Update_Defaults(p_scw_calibration);

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

#endif /* SCW_PRE_RUN_TEST_HPP*/
