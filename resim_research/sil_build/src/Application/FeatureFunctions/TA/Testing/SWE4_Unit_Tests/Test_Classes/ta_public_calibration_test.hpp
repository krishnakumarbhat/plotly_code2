#ifndef TA_PUBLIC_CALIBRATION_TEST_HPP
#define TA_PUBLIC_CALIBRATION_TEST_HPP

/**
 * @file ta_calibration_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for TA calibration unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include <gtest/gtest.h>

extern "C"
{
#include "ta_public_calibration.h"
}

/**
 * Class used to create a fixture for TA calibration tests
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ta_Public_Calibration_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */

   Ta_Public_Calibration_T ta_cal;


   void SetUp() override
   {
      Ta_Public_Cal_Update_Defaults(&ta_cal);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /*TA_PUBLIC_CALIBRATION_TEST_HPP*/
