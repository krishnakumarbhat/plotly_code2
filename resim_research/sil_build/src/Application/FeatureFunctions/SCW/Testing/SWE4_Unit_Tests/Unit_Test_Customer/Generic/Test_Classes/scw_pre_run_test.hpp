#ifndef SCW_PRE_RUN_TEST_HPP
#define SCW_PRE_RUN_TEST_HPP

/**
 * @file scw_pre_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for SCW generic pre run unit tests
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_guardrail_data_t.h"
#include "fbk_output.h"
#include "scw_core_calibration.h"
#include "scw_core_input_t.h"
#include "scw_iface.h"
#include "scw_input_t.h"
#include "scw_instance_t.h"
}

/**
 * Class used to create a fixture for SCW generic pre run tests
 * For writing two or more tests that operate on similar data, we can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Scw_Pre_Run_Test : public ::testing::Test
{
 protected:
   Scw_Instance_T scw_instance{};
   Scw_Core_Input_T *p_scw_core_input;
   Scw_Core_Calibration_T *p_scw_calibration;
   Scw_Persistent_T *p_scw_persistent;
   Fbk_Output_T fbk_output{};
   Scw_Input_T scw_input{};
   Pa_Data_T pa_data{};
   Fbk_Guardrail_Data_T *guardrail_data;
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   virtual void SetUp() override
   {
      fbk_output.p_pa_data = &pa_data;

      p_scw_core_input  = &(scw_instance.core_input);
      p_scw_calibration = &(scw_instance.calibration);
      p_scw_persistent  = &(scw_instance.persistent);

      guardrail_data = pa_data.guardrail_data;

      /* Initialize core input */
      p_scw_core_input->p_pa_data = &pa_data;

      /* Initialize calibration values */
      Scw_Core_Cal_Update_Defaults(p_scw_calibration);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown() override
   {
   }
};

#endif /* SCW_PRE_RUN_TEST_HPP*/
