#ifndef FBK_OBJ_TRAJ_PREDICTOR_TEST_HPP
#define FBK_OBJ_TRAJ_PREDICTOR_TEST_HPP

/**
 * @file fbk_obj_traj_predictor_test.hpp
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
#include "fbk_core_calibration.h"
#include "fbk_traj_predictor_t.h"
}

/**
 * Class used to create a fixture for FBK test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Fbk_Obj_Traj_Predictor_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */

   Fbk_Core_Calibration_T fbk_cals;
   Fbk_Object_Data_T tracker_data{};
   Fbk_Object_Predict_Data_T fbk_obj_data{};
   Fbk_Trajectory_T trajectory{};

   /* Iterator */
   uint8_t pred_step;

   void SetUp() override
   {
      Fbk_Core_Cal_Update_Defaults(&fbk_cals);

      /* Resets the ego trajectory properties */
      trajectory.f_trajectory_valid = FBK_FALSE;
      trajectory.n_prediction_steps = FBK_MAX_PREDICTION_STEPS;

      for (pred_step = FBK_ZERO_INT; pred_step < trajectory.n_prediction_steps; pred_step++)
      {
         Fbk_Init_Waypoint_with_Circle_Centers_Structure(&(trajectory.waypoint[pred_step]));
      }
      fbk_obj_data.obj_pred_speed_min           = 0.7f;
      fbk_obj_data.obj_shape_gain_fixed         = 1.0f;
      fbk_obj_data.obj_shape_gain_per_pred_step = 1.0f;
      fbk_obj_data.pred_step_dt                 = 0.0f;
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};
#endif /*FBK_OBJ_TRAJ_PREDICTOR_TEST_HPP*/
