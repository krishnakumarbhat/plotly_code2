#ifndef TA_FACTORY_TEST_HPP
#define TA_FACTORY_TEST_HPP

/**
 * @file ta_factory_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for TA unit tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include <gtest/gtest_pred_impl.h>

extern "C"
{
#include "ta_core_calibration.h"
#include "ta_types.h"
}

/**
 * Class used to create a fixture for TA test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ta_Factory_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */
   Fbk_Trajectory_T ego_trajectory{};
   Fbk_Trajectory_T obj_trajectory{};

   Fbk_Circle_Center_Offset_T circle_center_offset_structure{};
   Fbk_Waypoint_with_Circle_Centers_T waypoint_with_circle_centers_structure{};

   Ta_Core_Calibration_T ta_cal;

   void SetUp() override
   {
      Ta_Core_Cal_Update_Defaults(&ta_cal);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};
#endif /*TA_FACTORY_TEST_HPP*/
