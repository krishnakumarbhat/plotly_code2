#ifndef CED_CALIBRATION_TEST_HPP
#define CED_CALIBRATION_TEST_HPP

/**
 * @file ced_calibration_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for CED calibration unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "ced_core_calibration.h"
}

/**
 * Class used to create a fixture for CED calibration tests
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ced_Core_Calibration_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */

   Ced_Core_Calibration_T ced_cal;


   void SetUp() override
   {
      Ced_Core_Cal_Update_Defaults(&ced_cal);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /*CED_CALIBRATION_TEST_HPP*/
