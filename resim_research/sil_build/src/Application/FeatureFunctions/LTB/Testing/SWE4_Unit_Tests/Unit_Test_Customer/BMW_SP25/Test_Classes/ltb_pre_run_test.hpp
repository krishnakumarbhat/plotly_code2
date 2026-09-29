#ifndef LTB_PRE_RUN_TEST
#define LTB_PRE_RUN_TEST

/**
 * @file ltb_pre_run_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for BMW SP25 ltb pre run
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

#include "gtest/gtest_pred_impl.h" // IWYU pragma: keep
#include <gtest/gtest.h>           // IWYU pragma: keep

extern "C"
{
#include "fbk_object_data_t.h"
#include "fbk_output.h"
#include "fbk_vehicle_data_t.h"
#include "ltb_core_calibration.h"
#include "ltb_core_input_t.h"
#include "ltb_input_t.h"
#include "ltb_instance.h"
#include "ltb_state_machine.h"
}

/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ltb_Pre_Run_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Ltb_Core_Input_T *p_ltb_core_input{};
   Ltb_Input_T ltb_input{};
   Ltb_Instance_T ltb_instance{};
   Ltb_Core_Calibration_T &cals = ltb_instance.calibration;
   Fbk_Output_T fbk_output{};
   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;
   Ltb_State_T *p_ltb_current_state;

   void SetUp() override
   {
      Ltb_Core_Cal_Update_Defaults(&cals);
      object_data                       = data.object_data;
      p_vehicle_data                    = &data.vehicle_data;
      fbk_output.p_pa_data              = &data;
      ltb_instance.core_input.p_pa_data = &data;
      p_ltb_core_input                  = &ltb_instance.core_input;

      p_ltb_current_state = Ltb_Get_Current_State();
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

 protected:
};


#endif /* LTB_PRE_RUN_TEST */