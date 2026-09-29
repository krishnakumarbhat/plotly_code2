#ifndef CED_PRE_RUN_TEST
#define CED_PRE_RUN_TEST

/**
 * @file ced_pre_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for Generic Ced Pre Run
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

#include "gtest/gtest_pred_impl.h" // IWYU pragma: keep
#include <gtest/gtest.h>           // IWYU pragma: keep

extern "C"
{
#include "ced_core_calibration.h"
#include "ced_core_input_t.h"
#include "ced_input_t.h"
#include "ced_instance.h"
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "pa_context.h"
#include "pt_output_t.h"
}

/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ced_Pre_Run_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Pt_Output_T pt_output{};
   Ced_Instance_T ced_instance{};
   Ced_Core_Calibration_T *p_cals   = &ced_instance.calibration;
   Ced_Core_Input_T &ced_core_input = ced_instance.core_input;
   Ced_Input_T ced_input{};

   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data     = data.object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data = &data.vehicle_data;

   Fbk_Output_T fbk_output{};

   void SetUp() override
   {
      Ced_Core_Cal_Update_Defaults(p_cals);
      fbk_output.p_pa_data = &data;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};


#endif /* CED_PRE_RUN_TEST */