#ifndef ESA_CALIBRATION_TEST_HPP
#define ESA_CALIBRATION_TEST_HPP

/**
 * @file esa_calibration_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for ESA calibration unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest.h"
#include "gtest/gtest_pred_impl.h"
#include <tuple>

extern "C"
{
#include "esa_core_calibration.h"
#include "pa_reuse.h"
}

/**
 * Class used to create a fixture for ESA calibration tests
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Esa_Core_Calibration_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */

   Esa_Core_Calibration_T esa_cal;


   void SetUp() override
   {
      Esa_Core_Cal_Update_Defaults(&esa_cal);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /*ESA_CALIBRATION_TEST_HPP*/
