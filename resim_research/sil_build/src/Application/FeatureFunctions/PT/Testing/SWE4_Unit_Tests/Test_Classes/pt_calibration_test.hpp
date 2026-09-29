#ifndef PT_CALIBRATION_TEST_HPP
#define PT_CALIBRATION_TEST_HPP

/**
 * @file pt_calibration_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for PT calibration unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "pt_core_calibration.h"
}

/**
 * Class used to create a fixture for PT calibration tests
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Pt_Core_Calibration_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */

   Pt_Core_Calibration_T pt_cals;


   void SetUp() override
   {
      Pt_Core_Cal_Update_Defaults(&pt_cals);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /*PT_CALIBRATION_TEST_HPP*/
