#ifndef LTB_WARN_LOGIC_TEST_HPP
#define LTB_WARN_LOGIC_TEST_HPP

/**
 * @file ltb_warn_logic_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for LTB unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include <gtest/gtest_pred_impl.h>

extern "C"
{
#include "ltb_core_calibration.h"
#include "ltb_core_input_t.h"
#include "ltb_core_output_t.h"
#include "ltb_input_t.h"
#include "ltb_output_t.h"
#include "ltb_persistent_t.h"
#include "ltb_types.h"
}

/**
 * Class used to create a fixture for LTB test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ltb_Warn_Logic_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Ltb_Input_T ltb_input{};
   Ltb_Output_T ltb_output{};
   Ltb_Core_Output_T ltb_core_output_temp{};
   Ltb_Core_Input_T ltb_core_input{};
   Ltb_Core_Output_T ltb_core_output{};
   Ltb_Persistent_T ltb_persistent{};
   Ltb_Core_Calibration_T ltb_cals;
   Ltb_Object_T ltb_object{};
   Pa_Context_T ltb_context{};
   Pa_Data_T data{};

   void SetUp() override
   {
      Ltb_Core_Cal_Update_Defaults(&ltb_cals);

      /* Initialize context data */
      ltb_context.p_data = &data;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};
#endif /*LTB_WARN_LOGIC_TEST_HPP*/
