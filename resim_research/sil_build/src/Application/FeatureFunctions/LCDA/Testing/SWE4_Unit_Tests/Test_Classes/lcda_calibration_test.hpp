#ifndef LCDA_CALIBRATION_TEST_HPP
#define LCDA_CALIBRATION_TEST_HPP

/**
 * @file lcda_calibration_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for LCDA calibration unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "lcda_core_calibration.h"
}

/**
 * Class used to create a fixture for LCDA calibration tests
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Lcda_Core_Calibration_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */

   Lcda_Core_Calibration_T lcda_cals;


   void SetUp() override
   {
      Lcda_Core_Cal_Update_Defaults(&lcda_cals);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /*LCDA_CALIBRATION_TEST_HPP*/
