#ifndef TA_COMMON_FUNCTIONS_TEST_HPP
#define TA_COMMON_FUNCTIONS_TEST_HPP

/**
 * @file ta_common_functions_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for TA common functions unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include <gtest/gtest_pred_impl.h>

extern "C"
{
#include "fbk_macros.h"
#include "ta_core_calibration.h"
#include "ta_core_output_t.h"
#include "ta_persistent_t.h"
}

/**
 * Class used to create a fixture for TA test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ta_Common_Functions_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Ta_Core_Output_T ta_core_output{};
   Ta_Persistent_T ta_persistent{};
   Ta_Core_Calibration_T ta_cal;

   void SetUp() override
   {
      Ta_Core_Cal_Update_Defaults(&ta_cal);
      ta_cal.k_ta_f_skip_holding_for_single_alert_level_drop = FBK_FALSE;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};
#endif /* TA_COMMON_FUNCTIONS_TEST_HPP */
