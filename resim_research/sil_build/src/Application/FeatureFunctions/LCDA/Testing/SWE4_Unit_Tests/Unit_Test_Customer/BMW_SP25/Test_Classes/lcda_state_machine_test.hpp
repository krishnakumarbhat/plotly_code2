#ifndef LCDA_STATE_MACHINE_TEST_HPP
#define LCDA_STATE_MACHINE_TEST_HPP

/**
 * @file lcda_state_machine_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class Lcda_State_Machine_Test
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest.h>

extern "C"
{
#include "camera_data_t.h"
#include "fbk_guardrail_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "lcda_core_calibration.h"
#include "lcda_core_calibration_t.h"
#include "lcda_core_input_t.h"
#include "lcda_input_t.h"
#include "lcda_output_t.h"
#include "lcda_state_machine.h"
#include "pa_data.h"
}

class Lcda_State_Machine_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */

   Lcda_Input_T lcda_input{};
   Lcda_Core_Calibration_T lcda_cals;
   LCDA_FF_State_T lcda_current_state{};
   Fbk_Vehicle_Data_T *p_vehicle_data{};
   Pa_Context_T context{};
   Fbk_Object_Data_T *object_data;
   Pa_Data_T data{};
   virtual void SetUp() override
   {
      Lcda_Core_Cal_Update_Defaults(&lcda_cals);
      context.p_data = &data;
      object_data    = context.p_data->object_data;
      p_vehicle_data = &(context.p_data->vehicle_data);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown() override
   {
   }
};

#endif /* LCDA_STATE_MACHINE_TEST_HPP */
