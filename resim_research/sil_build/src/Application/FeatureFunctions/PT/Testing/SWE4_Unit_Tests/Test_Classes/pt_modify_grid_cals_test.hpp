#ifndef PT_MODIFY_GRID_CALS_TEST_HPP
#define PT_MODIFY_GRID_CALS_TEST_HPP

/**
 * @file pt_modify_grid_cals_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for PT interface functions unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "pt_core_calibration.h"
}

class Pt_Modify_Grid_Cals_Test : public ::testing::Test
{
 public:
   Pt_Core_Calibration_T pt_cals;

   /**
    * Here is the place for some initializations of public members
    */
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

 protected:
};

#endif /* PT_MODIFY_GRID_CALS_TEST_HPP */
