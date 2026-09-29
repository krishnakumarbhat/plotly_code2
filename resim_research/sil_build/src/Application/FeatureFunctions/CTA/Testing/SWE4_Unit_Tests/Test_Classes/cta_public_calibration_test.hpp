#ifndef CTA_CALIBRATION_TEST_HPP
#define CTA_CALIBRATION_TEST_HPP

/**
 * @file cta_calibration_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for CTA calibration unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <tuple>

extern "C"
{
#include "cta_public_calibration.h"
#include "pa_reuse.h"
}

/**
 * Class used to create a fixture for CTA calibration tests
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Cta_Public_Calibration_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */

   Cta_Public_Calibration_T cta_cal;


   void SetUp() override
   {
      Cta_Public_Cal_Update_Defaults(&cta_cal);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};


#endif /*CTA_CALIBRATION_TEST_HPP*/
