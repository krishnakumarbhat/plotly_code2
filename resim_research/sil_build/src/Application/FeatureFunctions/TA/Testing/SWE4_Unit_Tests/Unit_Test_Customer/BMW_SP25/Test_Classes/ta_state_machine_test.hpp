#ifndef TA_STATE_MACHINE_TEST_HPP
#define TA_STATE_MACHINE_TEST_HPP

/**
 * @file ta_state_machine_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class TA_State_Machine_Test
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest.h>

extern "C"
{
#include "fbk_guardrail_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "pa_data.h"
#include "ta_core_calibration_t.h"
#include "ta_core_input_t.h"
#include "ta_input_t.h"
#include "ta_output_t.h"
#include "ta_state_machine.h"
}

class TA_State_Machine_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */

   Ta_Input_T ta_input{};
   TA_FF_State_T ta_current_state{};
   Pa_Context_T ta_context{};
   Pa_Data_T data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;

   virtual void SetUp() override
   {
      /* Initialize context data */
      ta_context.p_data = &data;
      object_data       = ta_context.p_data->object_data;
      p_vehicle_data    = &(ta_context.p_data->vehicle_data);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown() override
   {
   }
};

#endif /* TA_STATE_MACHINE_TEST_HPP */