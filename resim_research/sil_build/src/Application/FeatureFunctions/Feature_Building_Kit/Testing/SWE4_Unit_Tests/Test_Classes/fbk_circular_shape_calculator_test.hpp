#ifndef FBK_CIRCULAR_SHAPE_CALCULATOR_TEST_HPP
#define FBK_CIRCULAR_SHAPE_CALCULATOR_TEST_HPP

/**
 * @file fbk_circular_shape_calculator_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for FBK unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include <gtest/gtest_pred_impl.h>

extern "C"
{
#include "fbk_circular_shape_calculator.h"
}

/**
 * Class used to create a fixture for FBK test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Fbk_Circular_Shape_Calculator_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */

   Fbk_Circle_Center_Offset_T circle_center_offset_structure{};
   Fbk_Ego_Predict_Data_T fbk_ego_data{};
   Fbk_Waypoint_with_Circle_Centers_T waypoint_res{};


   void SetUp() override
   {
      fbk_ego_data.ego_acceleration_weight                = 1.0f;
      fbk_ego_data.ego_shape_gain_fixed                   = 1.0f;
      fbk_ego_data.ego_circle_offset                      = 0.0f;
      fbk_ego_data.ego_circle_host_length_factor          = 1.0f;
      fbk_ego_data.ego_deceleration_weight                = 1.0f;
      fbk_ego_data.ego_yaw_angle_to_last_straight_section = 0.0f;
      fbk_ego_data.ego_max_pred_yaw_angle                 = 2.0f;
      fbk_ego_data.ego_shape_gain_per_pred_step           = 1.0f;
      fbk_ego_data.pred_step_dt                           = 0.0f;
      fbk_ego_data.ego_pred_const_velocity_pred_steps_min = 40u;
      fbk_ego_data.prediction_steps_max                   = 20u;
      fbk_ego_data.acc_weight_depend_on_alert_lvl         = FBK_FALSE;

      Fbk_Init_Waypoint_with_Circle_Centers_Structure(&waypoint_res);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};
#endif /*FBK_CIRCULAR_SHAPE_CALCULATOR_TEST_HPP*/
