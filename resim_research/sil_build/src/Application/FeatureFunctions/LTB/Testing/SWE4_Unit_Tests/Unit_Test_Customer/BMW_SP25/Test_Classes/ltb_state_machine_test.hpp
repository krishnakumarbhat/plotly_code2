#ifndef LTB_STATE_MACHINE_TEST_HPP
#define LTB_STATE_MACHINE_TEST_HPP
/**
 * @file ltb_state_machine_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for LTB state machine unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "fbk_vehicle_validation.h"
#include "ltb_core_calibration.h"
#include "ltb_core_calibration_t.h"
#include "ltb_core_input_t.h"
#include "ltb_input_t.h"
#include "ltb_state_machine.h"
}

/**
 * Class used to create a fixture for LTB state machine tests
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ltb_State_Machine_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */

   Ltb_Core_Calibration_T ltb_cals;
   Ltb_Input_T ltb_input{};
   Ltb_State_Flags_T state_flag{};
   Fbk_Vehicle_Data_T vehicle_data{};

   void SetUp() override
   {
      Ltb_Core_Cal_Update_Defaults(&ltb_cals);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /*LTB_STATE_MACHINE_TEST_HPP*/