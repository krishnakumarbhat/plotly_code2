#ifndef LCDA_COMMON_FUNCTIONS_TEST_HPP
#define LCDA_COMMON_FUNCTIONS_TEST_HPP

/**
 * @file lcda_common_functions_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class Lcda_Common_Functions_Test
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "fbk_object_data_t.h"
#include "lcda_core_calibration.h"
#include "lcda_core_input_t.h"
}

/**
 * Class used to create a fixture for Lcda_Common_Functions_Test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Lcda_Common_Functions_Test : public ::testing::Test
{
 protected:
   Lcda_Core_Calibration_T lcda_cals;
   Lcda_Core_Input_T lcda_core_input{};
   Fbk_Object_Data_T lcda_tracker_object{};

   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   virtual void SetUp()
   {
      /* Update default cals and set calibration pointer */
      Lcda_Core_Cal_Update_Defaults(&lcda_cals);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }
};

#endif /* LCDA_COMMON_FUNCTIONS_TEST_HPP */
