#ifndef ESA_POST_RUN_TEST_HPP
#define ESA_POST_RUN_TEST_HPP

/**
 * @file esa_post_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for ESA unit tests
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
#include "esa_input_t.h"
#include "esa_instance_t.h"
#include "esa_output_t.h"
#include "fbk_vehicle_data_t.h"
}

/**
 * Class used to create a fixture for ESA Tests
 * For writing two or more tests that operate on similar data, we can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Esa_Post_Run_Test : public ::testing::Test
{
 public:
   Esa_Instance_T esa_instance{};
   Esa_Persistent_T *p_esa_persistent;
   Esa_Core_Calibration_T *p_esa_calibration;
   Esa_Core_Input_T *p_esa_core_input;
   Esa_Core_Output_T *p_esa_core_output;
   Esa_Input_T esa_input{};
   Esa_Output_T esa_output{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;
   Pa_Data_T pa_data{};

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   void SetUp() override
   {
      p_esa_persistent  = &(esa_instance.persistent);
      p_esa_calibration = &(esa_instance.calibration);
      p_esa_core_input  = &(esa_instance.core_input);
      p_esa_core_output = &(esa_instance.core_output);

      p_vehicle_data = &(pa_data.vehicle_data);
      object_data    = pa_data.object_data;

      /* Initialize calibration values */
      Esa_Core_Cal_Update_Defaults(p_esa_calibration);

      p_esa_core_input->p_pa_data = &pa_data;
      p_vehicle_data              = &(pa_data.vehicle_data);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /* ESA_POST_RUN_TEST_HPP */
