#ifndef PT_COMMON_FUNCTIONS_TEST_HPP
#define PT_COMMON_FUNCTIONS_TEST_HPP

/**
 * @file pt_common_functions_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for PT common functions unit tests
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "pt_shared_unit_test_constructors.hpp"
#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "pa_reuse.h"
#include "pt_constants.h"
#include "pt_core_calibration.h"
#include "pt_persistent_t.h"
}

class Pt_Common_Functions_Test : public ::testing::Test, public Pt_Shared_Unit_Test_Constructors
{
 public:
   Pt_Core_Calibration_T cals;
   Pt_Path_T path{};
   Pt_Persistent_T pt_persistent{};
   float32_T p_grid_array[PT_NUM_GRID_POINTS];

   /**
    * Here is the place for some initializations of public members
    */
   void SetUp() override
   {
      Pt_Core_Cal_Update_Defaults(&cals);
      Pt_Update_Grid_Array_Defaults(p_grid_array);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }

 protected:
};

#endif /* PT_COMMON_FUNCTIONS_TEST_HPP */
