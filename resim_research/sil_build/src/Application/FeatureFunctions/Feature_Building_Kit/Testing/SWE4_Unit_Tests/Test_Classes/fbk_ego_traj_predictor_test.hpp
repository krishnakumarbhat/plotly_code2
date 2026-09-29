#ifndef FBK_EGO_TRAJ_PREDICTOR_TEST_HPP
#define FBK_EGO_TRAJ_PREDICTOR_TEST_HPP

/**
 * @file fbk_ego_traj_predictor_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class for FBK unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include <gtest/gtest_pred_impl.h>

extern "C"
{
#include "fbk_core_calibration.h"
#include "fbk_ego_traj_predictor.c"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "ml_vector_2d_t.h"
#include "pa_context.h"
}

/**
 * Class used to create a fixture for FBK test
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Fbk_Ego_Traj_Predictor_Test : public ::testing::Test
{
 public:
   /**
    * Function used to prepare the objects for each test
    * Here is the place for some initializations of public members
    */

   Fbk_Waypoint_with_Circle_Centers_T waypoint_with_circle_centers{};

   Fbk_Core_Calibration_T fbk_cal;
   Fbk_Ego_Predict_Data_T fbk_ego_data{};
   Fbk_Object_Data_T *object_data;
   Fbk_Trajectory_T ego_trajectory{};
   Fbk_Vehicle_Data_T *p_vehicle_data;
   Pa_Context_T pa_context{};
   Pa_Data_T data{};
   static Fbk_Ego_Traj_Predictor_Instance_T Ego_Traj_Instance;

   /* Iterator */
   uint8_t pred_step;

   void SetUp() override
   {
      Ego_Traj_Instance.F_Host_Circle_Props_Initialized   = FBK_FALSE;
      waypoint_with_circle_centers.waypoint_coordinates.x = 0.0f;
      waypoint_with_circle_centers.waypoint_coordinates.y = 0.0f;

      fbk_ego_data.ego_acceleration_weight                = 1.0f;
      fbk_ego_data.ego_shape_gain_fixed                   = 1.0f;
      fbk_ego_data.ego_circle_offset                      = 0.0f;
      fbk_ego_data.ego_circle_host_length_factor          = 1.0f;
      fbk_ego_data.ego_deceleration_weight                = 1.0f;
      fbk_ego_data.ego_yaw_angle_to_last_straight_section = 0.0f;
      fbk_ego_data.ego_max_pred_yaw_angle                 = 2.0f;
      fbk_ego_data.ego_shape_gain_per_pred_step           = 1.0f;
      fbk_ego_data.pred_step_dt                           = 0.1f;
      fbk_ego_data.ego_pred_const_velocity_pred_steps_min = 40u;
      fbk_ego_data.prediction_steps_max                   = 20u;
      fbk_ego_data.acc_weight_depend_on_alert_lvl         = FBK_FALSE;

      Fbk_Core_Cal_Update_Defaults(&fbk_cal);

      /* Initialize context data */
      pa_context.p_data = &data;
      object_data       = pa_context.p_data->object_data;
      p_vehicle_data    = &(pa_context.p_data->vehicle_data);

      /* Resets the ego trajectory properties */
      ego_trajectory.f_trajectory_valid = FBK_FALSE;
      ego_trajectory.n_prediction_steps = FBK_MAX_PREDICTION_STEPS;

      for (pred_step = FBK_ZERO_INT; pred_step < ego_trajectory.n_prediction_steps; pred_step++)
      {
         Fbk_Init_Waypoint_with_Circle_Centers_Structure(&(ego_trajectory.waypoint[pred_step]));
      }
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   void TearDown() override
   {
   }
};
#endif /*FBK_EGO_TRAJ_PREDICTOR_TEST_HPP*/
