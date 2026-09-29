#ifndef LTB_CALIBRATION_TEST_HPP
#define LTB_CALIBRATION_TEST_HPP

/**
 * @file ltb_calibration_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for LTB calibration unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "ltb_core_calibration.h"
}

/**
 * Class used to create a fixture for LTB calibration tests
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ltb_Core_Calibration_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */

   Ltb_Core_Calibration_T ltb_cals;


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

#endif /*LTB_CALIBRATION_TEST_HPP*/