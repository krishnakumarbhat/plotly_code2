#ifndef SCW_CALIBRATION_TEST_HPP
#define SCW_CALIBRATION_TEST_HPP

/**
 * @file scw_calibration_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for SCW calibration unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "gmock/gmock.h"
#include "gtest/gtest.h"

extern "C"
{
#include "scw_core_calibration.h"
}

/**
 * Class used to create a fixture for SCW calibration tests
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Scw_Core_Calibration_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */

   Scw_Core_Calibration_T scw_cals;


   void SetUp() override
   {
      Scw_Core_Cal_Update_Defaults(&scw_cals);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /*SCW_CALIBRATION_TEST_HPP*/
