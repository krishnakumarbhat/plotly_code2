#ifndef TA_BMW_DIAGNOSTIC_TEST_HPP
#define TA_BMW_DIAGNOSTIC_TEST_HPP

/**
 * @file ta_bmw_diagnostic_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for BMW SRR5 TA diagnostic tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include <gtest/gtest.h> // IWYU pragma: keep
#include <gtest/gtest_pred_impl.h>
#include <memory>

extern "C"
{
#include "ta_core_calibration.h"
#include "ta_input_t.h"
}

/**
 * Class used to create a fixture for BMW SRR5 TA diagnostic test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ta_Bmw_Diagnostic_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Ta_Input_T ta_input;
   Ta_Core_Calibration_T ta_cals;

   void SetUp() override
   {
      Ta_Core_Cal_Update_Defaults(&ta_cals);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /* TA_BMW_DIAGNOSTIC_TEST_HPP */
