#ifndef RECW_CALIBRATION_TEST_HPP
#define RECW_CALIBRATION_TEST_HPP

/**
 * @file recw_calibration_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for RECW calibration unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest.h"

extern "C"
{
#include "recw_core_calibration.h"
}

/**
 * Class used to create a fixture for RECW calibration tests
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Recw_Core_Calibration_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */

   Recw_Core_Calibration_T recw_cals;


   void SetUp() override
   {
      Recw_Core_Cal_Update_Defaults(&recw_cals);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};

#endif /*RECW_CALIBRATION_TEST_HPP*/
