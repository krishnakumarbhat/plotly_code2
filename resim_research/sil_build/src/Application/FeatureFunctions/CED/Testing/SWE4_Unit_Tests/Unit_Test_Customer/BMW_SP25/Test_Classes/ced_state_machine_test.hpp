#ifndef CED_STATE_MACHINE_TEST
#define CED_STATE_MACHINE_TEST

/**
 * @file ced_state_machine_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for bmw sp25 state machine
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest.h>

extern "C"
{
#include "ced_core_calibration.h"
#include "ced_core_calibration_t.h"
#include "ced_state_machine.h"
#include "fbk_vehicle_data_t.h"
}


/**
 * Class used to create a fixture
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ced_State_Machine_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Ced_Core_Calibration_T ced_cal;

   Pa_Data_T data{};

   Fbk_Object_Data_T *object_data;
   Fbk_Vehicle_Data_T *p_vehicle_data;

   Ced_Input_T ced_input{};

   CED_FF_STATE_T ced_current_state{};

   void SetUp() override
   {
      Ced_Core_Cal_Update_Defaults(&ced_cal);

      object_data    = data.object_data;
      p_vehicle_data = &(data.vehicle_data);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};


#endif /* CED_STATE_MACHINE_TEST */