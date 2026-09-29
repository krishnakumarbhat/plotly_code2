#ifndef TA_INTERSECTION_ANALYZER_TEST_HPP
#define TA_INTERSECTION_ANALYZER_TEST_HPP

/**
 * @file ta_intersection_analyzer_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for TA unit tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include <gtest/gtest.h> // IWYU pragma: keep
#include <gtest/gtest_pred_impl.h>

extern "C"
{
#include "fbk_circular_shape_calculator.h"
#include "ta_core_calibration.h"
#include "ta_types.h"
}

class Ta_Intersection_Analyzer_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */

   Ta_Core_Calibration_T ta_cal;
   Fbk_Waypoint_with_Circle_Centers_T ego{};
   Fbk_Waypoint_with_Circle_Centers_T obj{};

   void SetUp() override
   {
      Ta_Core_Cal_Update_Defaults(&ta_cal);

      Fbk_Init_Waypoint_with_Circle_Centers_Structure(&ego);
      Fbk_Init_Waypoint_with_Circle_Centers_Structure(&obj);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }


 protected:
};
#endif
